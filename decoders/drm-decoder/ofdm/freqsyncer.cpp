#
/*
 *    Copyright (C) 2013 .. 2017
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair Computing
 *
 *    This file is part of the drm receiver
 *
 *    drm receiver is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    drm receiver is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with drm receiver; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	"freqsyncer.h"
#include	"reader.h"
#include	"drm-decoder.h"
#include	"basics.h"
#include	"referenceframe.h"

//
struct testCells {
	int	index;
	int	phase;
};

struct testCells testCellsforModeA [] = {
        {18, 205},
        {54, 836},
        {72, 215},
        {-1, -1}
};

struct testCells testCellsforModeB [] = {
        {16, 331},
        {48, 651},
        {64, 555},
        {-1, -1}
};

struct testCells testCellsforModeC [] = {
        {11, 214},
        {33, 392},
        {44, 242},
        {-1, -1}
};

struct testCells testCellsforModeD [] = {
        { 7, 788},
        {21, 1014},
        {28, 332},
        {-1, -1}
};
//	The frequency shifter is in steps of 0.01 Hz
	freqSyncer::freqSyncer (Reader		*b,
	                        smodeInfo	*m,
	                        int32_t		sampleRate,	
	                        drmDecoder	*mr):
	                                 theShifter (100 * sampleRate) {
int16_t	i;
	this	-> theReader	= b;
	this	-> Mode		= m -> Mode;
	this	-> Spectrum	= m -> Spectrum;
	this	-> sampleRate	= sampleRate;
	this	-> theAngle	= 0;
	this	-> Tu		= Tu_of (Mode);
	this	-> Ts		= Ts_of (Mode);
	this	-> Tg		= Tg_of (Mode);
	this	-> displayCount	= 0;
	this	-> N_symbols	= symbolsperFrame (Mode);

	struct testCells *base =
	            Mode == Mode_A ? testCellsforModeA :
	            Mode == Mode_B ? testCellsforModeB :
	            Mode == Mode_C ? testCellsforModeC : testCellsforModeD;

	this	-> k_pilot1 = base [0]. index + Tu / 2;
	this	-> k_pilot2 = base [1]. index + Tu / 2;
	this	-> k_pilot3 = base [2]. index + Tu / 2;

	this	-> symbolBuffer	= new std::complex<float> *[N_symbols];
	for (i = 0; i < N_symbols; i ++)
	   symbolBuffer [i] = new std::complex<float> [Tu_of (Mode)];
	fft_vector		= (std::complex<float> *)
	                               fftwf_malloc (Tu_of (Mode) *
	                                            sizeof (fftwf_complex));
	hetPlan			= fftwf_plan_dft_1d (Tu_of (Mode),
	                    reinterpret_cast <fftwf_complex *>(fft_vector),
	                    reinterpret_cast <fftwf_complex *>(fft_vector),
	                    FFTW_FORWARD, FFTW_ESTIMATE);
	connect (this, SIGNAL (show_fineOffset (float)),
	         mr, SLOT (show_fineOffset (float)));
	connect (this, SIGNAL (show_coarseOffset (float)),
	         mr, SLOT (show_coarseOffset (float)));
	connect (this, SIGNAL (show_timeDelay	(float)),
	         mr, SLOT (show_timeDelay (float)));
	connect (this, SIGNAL (show_timeOffset	(float)),
	         mr, SLOT (show_timeOffset (float)));
	connect (this, SIGNAL (show_clockOffset (float)),
	         mr, SLOT (show_clockOffset (float)));
	connect (this, SIGNAL (show_angle (float)),
	         mr, SLOT (show_angle (float)));
}

		freqSyncer::~freqSyncer (void) {
int16_t	i;
	fftwf_free (fft_vector);
	fftwf_destroy_plan (hetPlan);
	for (i = 0; i < N_symbols; i ++)
	   delete[]  symbolBuffer [i];
	delete[] symbolBuffer;
}

bool freqSyncer::frequencySync (smodeInfo *m) {
int16_t	i;
int32_t	localIndex	= 0;
float	occupancyIndicator [6];
uint8_t	spectrum;

	theReader	-> waitfor (N_symbols * Ts + 4 * Ts);
//
//	first, load spectra for the first N_symbol symbols
//	into the (circular) buffer "symbolBuffer",
//	However: do not move the "currentIndex" in the Reader,
//	i.e. do not consume the words from the input
	for (i = 0; i < N_symbols; i ++) {
	 int16_t  time_offset_integer =
	          getWord (theReader -> data,
	                   theReader -> bufSize,
	                   theReader -> currentIndex + localIndex,
	                   i,
	                   0,
	                   m -> timeOffset_fractional);
	   localIndex += Ts + time_offset_integer;
	}
//
//	our version of get_zeroBin returns the "bin" number
//	of the bin with the highest energy level.
	int32_t	binNumber;
	binNumber = get_zeroBin (0);
	m -> freqOffset_integer	= binNumber * sampleRate / Tu;

	localIndex	= 0;
	for (i = 0; i < N_symbols; i ++) {
	 int16_t  time_offset_integer =
	          getWord (theReader -> data,
	                   theReader -> bufSize,
	                   theReader -> currentIndex + localIndex,
	                   i,
	                   m -> freqOffset_integer,
	                   m -> timeOffset_fractional);
	   localIndex += Ts + time_offset_integer;
	}

	fprintf (stderr, "bin 0 = %d\n", binNumber);
//	binNumber = binNumber < 0 ? binNumber + Tu : binNumber;
	for (i = 0; i <= 3; i ++) 
	   occupancyIndicator [i] = get_spectrumOccupancy (i, 0);

	float tmp1	= 0.0;
	for (spectrum = 0; spectrum <= 3; spectrum ++) {	
	   fprintf (stderr, "spectrum %d, indicator %f\n",
	                     spectrum, occupancyIndicator [spectrum]);
	   if (occupancyIndicator [spectrum] >= tmp1) {
	      tmp1 = occupancyIndicator [spectrum];
	      m -> Spectrum = spectrum;
	   }
	}
	fprintf (stderr, "spectrum wordt gezet op %d\n", m -> Spectrum);
	return true;
}

//
//	get_zeroBin cooperates with the local version of getWord.
//	It uses the two dimensional array "symbolBuffer" with
//	in the rows the computed spectra of the last N_symbols ofdm words,
//	starting at start (i.e. circular)

static inline
JAN	square (std::complex<JAN> v) {
	return real (v * conj (v));
}

int32_t	freqSyncer::get_zeroBin (int16_t start) {
int16_t i, j;
std::complex<float> correlationSum [Tu];
float	abs_sum [Tu];
float	squares [Tu];
//
//
//	assuming we do not have freq sync pilots, it is possible to
//	compute the coarse frequency offset, i.e. the "null" carrier
//	by looking at pilot carriers at corresponding places,
//	shifted one window.
//	Problem is of course that we do not know the "first"
//	symbol.
//
	memset (correlationSum, 0, Tu * sizeof (std::complex<float>));
	memset (abs_sum, 0, Tu * sizeof (float));
	memset (squares, 0, Tu * sizeof (float));

//	accumulate phase diffs of all carriers in subsequent symbols
	for (j = start + 1; j < start + N_symbols; j++) {
	   int16_t jmin1 	= (j - 1) % N_symbols;
	   int16_t jj		= j % N_symbols;
	   for (i = 0; i < Tu; i++) {
	      std::complex<float> tmp1 = symbolBuffer [jmin1][i] *
	                                        conj (symbolBuffer [jj][i]);
	      correlationSum [i] += tmp1;
	      squares [i] += square (symbolBuffer [jmin1][i]) +
	                                square (symbolBuffer [jj][i]);
	   }
	}
//
	for (i = 0; i < Tu; i++) 
	   abs_sum [i] = abs (squares [i] - abs (correlationSum [i]));

	float	lowest		= 1.0E20;
	int	dcOffset	= 0;
	float	highest		= 0;
	int	altOffset	= 0;

	for (int i = -10; i < 10; i ++) {
	   float h = squares [k_pilot1 + i - 2] + squares [k_pilot1 + i - 1] +
	             squares [k_pilot1 + i + 1] + squares [k_pilot1 + i + 2];
	   float sum_1 = 4 * squares [k_pilot1 + i] / h;
	   h = squares [k_pilot2 + i - 2] + squares [k_pilot2 + i - 1] +
	             squares [k_pilot2 + i + 1] + squares [k_pilot2 + i + 2];
	   float sum_2 = 4 * squares [k_pilot2 + i] / h;
	   h = squares [k_pilot3 + i - 2] + squares [k_pilot3 + i - 1] +
	             squares [k_pilot3 + i + 1] + squares [k_pilot3 + i + 2];
	   float sum_3 = 4 * squares [k_pilot3 + i] / h;
	   if ((sum_1 > 1) && (sum_2 > 1) && (sum_3 > 1)) {
	      if (sum_1 + sum_2 + sum_3 > highest) {
	         highest = sum_1 + sum_2 + sum_3;
	         altOffset = i;
	      }
	   }
	}
//	recall that the pilots are relative to -Tu / 2
	for (i = - 10; i < 10; i ++) {
	   float sum = abs_sum [k_pilot1 + i] +
	               abs_sum [k_pilot2 + i] +
	               abs_sum [k_pilot3 + i];
	   if (sum > highest) {
	      dcOffset	= i;
	      highest	= sum;
	   }
	}
	return altOffset;
	return dcOffset;
}

JAN	freqSyncer::get_spectrumOccupancy (uint8_t spectrum,
	                                   int16_t baseBin) {
int16_t	i, j;
int16_t K_min_ = Kmin (Mode, spectrum);
int16_t K_max_ = Kmax (Mode, spectrum);

	if (K_min_ == K_max_) 	// should not happen
	   return 0;

//	now we go

//	The indices of the "lowest" and the "highest" carrier.
	int K_min_indx		= (Tu / 2 + baseBin + K_min_) % (Tu);
	int K_max_indx		= (Tu / 2 + baseBin + K_max_) % (Tu);

	float tmp3	= 0;
	float tmp4	= 0;
	float tmp5	= 0;
	float tmp6	= 0;

	for (i = 0; i < N_symbols; i ++) {
//	near the carrier with the lowest index
	   for (j = 0; j < 10; j ++) {
	      int ind1 = (K_min_indx - 2 - j) % Tu;
	      int ind2 = (K_min_indx + 2 + j) % Tu;
	      if (ind1 < 0) 
	         fprintf (stderr, "fout met %d %d\n", K_min_indx, j);
	      tmp3 += abs (real (symbolBuffer [i][ind1] *
	                                  conj (symbolBuffer [i][ind1])));
	      tmp4 += abs (real (symbolBuffer [i][ind2] *
	                                  conj (symbolBuffer [i][ind2])));
	   }
//	near the carrier with the highest index
	   for (j = 0; j < 20; j ++) {
	      int ind1 = (K_max_indx - 2 - j) % Tu;
	      int ind2 = (K_max_indx + 2 + j) % Tu;
	      tmp5 += abs (real (symbolBuffer [i][ind1] *
	                                 conj (symbolBuffer [i][ind1])));
	      tmp6 += abs (real (symbolBuffer [i][ind2] *
	                                 conj (symbolBuffer [i][ind2])));
	   }
	}

	float energy_ratio_K_min_to_K_min_p4;
	float energy_ratio_K_max_to_K_max_p4;
//
//	some safety measure, we assume that the data that is to
//	supposed to be in the carriers with energy has substantially
//	higher value that the "outside" data
	energy_ratio_K_min_to_K_min_p4 = tmp4 / tmp3;
	energy_ratio_K_max_to_K_max_p4 = tmp5 / tmp6;
	return energy_ratio_K_max_to_K_max_p4 +
	       energy_ratio_K_min_to_K_min_p4;
}
//
//	In this local version of getWord, the input buffer is only looked
//	at, not read!! It is called by the frequency synchronizer
//	No reduction of the output to the Kmin .. Kmax useful
//	carriers is made, but the order of the low-high freqencies
//	is change to reflect the "lower .. higher" frequencies in order.
int16_t	freqSyncer::getWord (std::complex<JAN> *buffer,
	                     int32_t		bufSize,
	                     int32_t		theIndex,
	                     int16_t		wordNumber,
	                     int		intOffset,
	                     JAN		offsetFractional) {
std::complex<float> temp [Ts];
int16_t		i;
uint32_t	bufMask	= bufSize - 1;
std:;complex<float> angle	= std::complex<float> (0, 0);

	
//	To take into account the fractional timing difference,
//	we do some interpolation between samples in the time domain
	int f	= (int)(floor (theIndex)) & bufMask;
	if (offsetFractional < 0) {
	   offsetFractional = 1 + offsetFractional;
	   f -= 1;
	}

	for (i = 0; i < Ts; i ++) {
	   std::complex<JAN> een = buffer [(f + i) & bufMask];
	   std::complex<JAN> twee = buffer [(f + i + 1) & bufMask];
	   temp [i] = cmul (een, 1 - offsetFractional) +
	              cmul (twee, offsetFractional);
	}

	theShifter. do_shift (temp, Ts, intOffset * 100);
//	Now: estimate the fine grain offset.
	for (i = 0; i < Tg; i ++)
	   angle += conj (temp [Tu + i]) * temp [i];
//	simple averaging:
	theAngle	= 0.9 * theAngle + 0.1 * arg (angle);

//	offset in Hz / 100
	float offset	= theAngle / (2 * M_PI) * 100 * sampleRate / Tu;
	if (++displayCount > 20) {
	   displayCount = 0;
	   show_coarseOffset	(intOffset);
	   show_fineOffset	(- offset / 100);
	   show_angle		(arg (angle));
	   show_timeDelay	(offsetFractional);
	}

	if (!isnan (offset))	// precaution to handle undefines
	   theShifter. do_shift (temp, Ts, -offset);

//	and extract the Tu set of samples for fft processsing
	memcpy (fft_vector, &temp [Tg], Tu * sizeof (std::complex<float>));
	fftwf_execute (hetPlan);
	memcpy (symbolBuffer [wordNumber],
	        &fft_vector [Tu / 2], Tu / 2 * sizeof (std::complex<float>));
	memcpy (&symbolBuffer [wordNumber] [Tu / 2],
	        fft_vector , Tu / 2 * sizeof (std::complex<float>));
	return 0;
}

