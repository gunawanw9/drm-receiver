#
/*
 *    Copyright (C) 2015 .. 2017
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
#include	"word-collector.h"
#include	"reader.h"
#include	"drm-decoder.h"
#include	"basics.h"

#include	"fft-complex.h"
//	The wordCollector will handle segments of a given size,
//	do all kinds of frequency correction (timecorrection
//	was done in the syncer) and map them onto ofdm words.
//	
//	The caller just calls upon "getWord" to get a new ofdm word
#define	NRSYMBOLS	24
#define	EPSILON		1.0E-10
//	The frequency shifter is in steps of 0.01 Hz
	wordCollector::wordCollector (drmDecoder *mr,
	                              Reader	*b,
	                              smodeInfo	*modeInf,
	                              int32_t	sampleRate):
	                                 theShifter (100 * sampleRate) {
	this	-> master	= mr;
	this	-> buffer	= b;
	this	-> bufMask	= buffer -> bufSize - 1;
	this	-> sampleRate	= sampleRate;
	this	-> modeInf	= modeInf;
	this	-> Mode		= modeInf -> Mode;
	this	-> Spectrum	= modeInf -> Spectrum;
	this	-> theAngle	= 0;
	this	-> theAngle_2	= 0;
	this	-> Tu		= Tu_of (Mode);
	this	-> Ts		= Ts_of (Mode);
	this	-> Tg		= Tg_of (Mode);
	this	-> K_min	= Kmin	(Mode, Spectrum);
	this	-> K_max	= Kmax	(Mode, Spectrum);
	this	-> displayCount	= 0;
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

		wordCollector::~wordCollector (void) {
}

static JAN theOffset= 0;
//	when starting up, we "borrow" the precomputed frequency offset
//	and start building up the spectrumbuffer.
//	
void	wordCollector::getWord (std::complex<JAN>	*out,
	                        int32_t		initialFreq,
	                        JAN		offsetFractional) {
std::complex<JAN>	temp [Ts + 100];
std::complex<JAN>	angle	= std::complex<JAN> (0, 0);
int	f	= buffer -> currentIndex;

	buffer		-> waitfor (Ts + Ts / 2);
	theOffset = 0;

//	correction of the time offset by interpolation
	for (int i = 0; i < Ts; i ++) {
	   std::complex<JAN> one = buffer ->  data [(f + i) & bufMask];
	   std::complex<JAN> two = buffer ->  data [(f + i + 1)& bufMask];
//	   temp [i] = one;
	   temp [i] = cmul (one, 1 - offsetFractional) +
	                  cmul (two, offsetFractional);
	}

//	And we shift the bufferpointer here
	buffer -> currentIndex = (f + Ts) & bufMask;

//	theShifter. do_shift (temp, Ts, 100 * initialFreq);
//	Now: determine the fine grain offset.
	for (int i = 0; i < Tg; i ++)
	   angle += conj (temp [Tu + i]) * temp [i];
//	simple averaging
	theAngle	= 0.6 * theAngle + 0.4 * arg (angle);
	theAngle_2	= theAngle;
//
//	offset  (and shift) in Hz / 100
	JAN offset	= theAngle / (2 * M_PI) * 100 * sampleRate / Tu;
	if (!isnan (offset))	// precaution to handle undefines
	   theShifter. do_shift (temp, Ts,
	                     100 * initialFreq - offset);

	if (++displayCount > 20) {
	   displayCount = 0;
	   show_coarseOffset	(initialFreq);
	   show_fineOffset	(- offset / 100);
	   show_angle		(arg (angle));
	   show_timeDelay	(offsetFractional);
	}

	fft_and_extract (&temp [Tg], out);
}
//
//	The getWord as below is used in the main loop, to obtain
//	a next ofdm word
void	wordCollector::getWord (std::complex<JAN>	*out,
	                        int32_t		initialFreq,
	                        bool		firstTime,
	                        JAN		offsetFractional,
	                        JAN		angle,
	                        JAN		clockOffset) {
std::complex<JAN>	temp [Ts + 100];
int	f		= buffer -> currentIndex;

	buffer		-> waitfor (Ts + Ts / 2);

	theOffset	= offsetFractional + clockOffset;
//	if  (theOffset < -0.5) {
//	   theOffset += 1;
//	   fprintf (stderr, "-1 due to fractionalOffset %f\n", offsetFractional);
//	   buffer -> currentIndex --;
//	}
//	else
//	if (theOffset >= 0.5 ) {
//	   theOffset -= 1;
//	   fprintf (stderr, "+1 due to fractionalOffset %f\n", offsetFractional);
//	   buffer -> currentIndex ++;
//	}
//
//	if (theOffset < 0) {
//	   f --;
//	   theOffset += 1;
//	}

	for (int i = 0; i < Ts; i ++) {
	   std::complex<JAN> one = buffer -> data [(f + i) & bufMask];
	   std::complex<JAN> two = buffer -> data [(f + i + 1) & bufMask];
	   temp [i] = 
	             cmul (one, 1 - theOffset) + cmul (two, theOffset);
	}

//	And we adjust the bufferpointer here
	buffer -> currentIndex = (buffer -> currentIndex + Ts) & bufMask;

	std::complex<float> phaseError = std::complex<float> (0, 0);
//	Now: determine the fine grain offset.
//	for (int i = 1; i < Tg - 1; i ++)
//	   phaseError += conj (temp [Tu + i]) * temp [i];
//	simple averaging
//	theAngle_2	= 0.9 * theAngle + 0.1 * arg (phaseError);

//	correct the phase
	theAngle	= theAngle - 0.1 * angle;

//	offset in 0.01 * Hz
	JAN freqOff          = theAngle / (2 * M_PI) * 100 * sampleRate / Tu;
	if (!isnan (freqOff)) { // precaution to handle undefines
	   theShifter. do_shift (temp, Ts,
	                         100 * modeInf -> freqOffset_integer -
	                                         freqOff);
	}
	else
	   theAngle = 0;

	if (++displayCount > 20) {
	   displayCount = 0;
	   show_coarseOffset	(initialFreq);
	   show_fineOffset	(- freqOff / 100);
	   show_angle		(angle);
	   show_timeOffset	(theOffset);
	   show_clockOffset	(Ts * clockOffset);
	}

	fft_and_extract (&temp [Tg], out);
}

void	wordCollector::fft_and_extract (std::complex<JAN> *in,
	                                std::complex<JAN> *out) {
//	and extract the Tu set of samples for fft processsing
//std::complex<JAN> temp [Tu];

//	memcpy (fft_vector, in, Tu * sizeof (std::complex<JAN>));
	Fft_transform (in, Tu, false);
//	fftwf_execute (hetPlan);
//	extract the "useful" data
	if (K_min < 0) {
	   memcpy (out,
	           &in [Tu + K_min],
	           - K_min * sizeof (std::complex<JAN>));
	   memcpy (&out [- K_min],
	           &in [0],
	           (K_max + 1) * sizeof (std::complex<JAN>));
	}
	else
	   memcpy (out,
	           &in [K_min],
	           (K_max - K_min + 1) * sizeof (std::complex<JAN>));
}

JAN wordCollector::get_timeOffset	(int nrSymbols, int range, int *o) {
int	b [nrSymbols];
std::complex<JAN> summedCorrelations [nrSymbols * Ts - Tu_of (Mode_D)];
JAN summedSquares [nrSymbols * Ts - Tu_of (Mode_D)];

	buffer -> waitfor (2 * nrSymbols * Ts + Ts);
	JAN avgOff	= get_intOffset (0, nrSymbols, range);

	for (int i = 0; i < nrSymbols * Ts - Tu; i ++) {
	   summedCorrelations [i] = std::complex<JAN> (0, 0);
	   summedSquares [i]	= 0;
	   for (int j; j < Tg; j ++) {
	      std::complex<JAN> f1        =
                           buffer -> data [(buffer -> currentIndex + i + j) & bufMask];
              std::complex<JAN> f2        =
                           buffer -> data [(buffer -> currentIndex + i + Tu + j) & bufMask];
              summedCorrelations [i] += f1 * conj (f2);
              summedSquares      [i] += real (f1 * conj (f1)) +
                                             real (f2 * conj (f2));
           }
        }

	int16_t index = Tg + avgOff + Ts / 2;
	JAN	mmse = 0;
        for (int j = 0; j < (nrSymbols - 2); j++) {
           JAN minValue       = 1000000.0;
           for (int i = 0; i < Ts; i++) {
              std::complex<JAN> gamma  = summedCorrelations [(index + i)];
              JAN squareTerm    = (JAN) (0.5 * (EPSILON +
                                         summedSquares [index + i]));
              JAN mmse = squareTerm - 2 * abs (gamma);
              if (mmse < minValue) {
                 minValue = mmse;
                 b [j] = i;
              }
           }
           index += Ts;
        }

	JAN   sumx    = 0.0;
	JAN   sumy    = 0.0;
	JAN   sumxx   = 0.0;
	JAN   sumxy   = 0.0;

	for (int i = 0; i < nrSymbols - 1; i++) {
	   sumx += (JAN) i;
	   sumy += (JAN) b [i];
	   sumxx += (JAN) i * (JAN) i;
	   sumxy += (JAN) i * (JAN) b [i];
	}

	JAN boffs;
	boffs = (JAN) ((sumy * sumxx - sumx * sumxy) /
	                    ((nrSymbols - 1) * sumxx - sumx * sumx));
	JAN timeOffset	= fmodf ((Tg + Ts) / 2 + avgOff - boffs - 1,
	                                               Ts);
	return timeOffset;
}
 
//JAN	wordCollector::get_timeOffset	(int nrSymbols,
//	                                 int range, int *offs) {
//int16_t b [nrSymbols];
//
//	buffer -> waitfor (2 * nrSymbols * Ts + Ts);
//	*offs	= get_intOffset (0, nrSymbols, range);
//	for (int i = 0; i < nrSymbols; i ++)
//	   b [i] = get_intOffset (i * Ts + *offs, nrSymbols, range);
//
//	JAN   sumx    = 0.0;
//        JAN   sumy    = 0.0;
//        JAN   sumxx   = 0.0;
//        JAN   sumxy   = 0.0;
//
//        for (int i = 0; i < nrSymbols - 1; i++) {
//           sumx += (JAN) i;
//           sumy += (JAN) b [i];
//           sumxx += (JAN) i * (JAN) i;
//           sumxy += (JAN) i * (JAN) b [i];
//        }
//
//        JAN bestOffsets;
//        bestOffsets = (JAN) ((sumy * sumxx - sumx * sumxy) /
//                         ((nrSymbols - 1) * sumxx - sumx * sumx));
//
//	return bestOffsets;
//}

int	wordCollector::get_intOffset	(int base,
	                                 int nrSymbols, int range) {
int	bestIndex = -1;
double	min_mmse = 10E20;

	for (int i = base - range / 2; i < base + range / 2; i ++) {
	   int index = (buffer -> currentIndex + i) & bufMask;
	   double mmse = compute_mmse (index, nrSymbols);
	   if (mmse < min_mmse) {
	      min_mmse = mmse;
	      bestIndex = i;
	   }
	}
	
	return bestIndex;
}

double	wordCollector::compute_mmse (int starter, int nrSymbols) {
std::complex<JAN>	gamma = std::complex<JAN> (0, 0);
double	squares = 0;
int32_t		bufMask	= buffer -> bufSize - 1;

	buffer -> waitfor (nrSymbols * Ts + Ts);
	for (int i = 0; i < nrSymbols - 1; i ++) {
	   int startSample = starter + i * Ts;
	   for (int j = 0; j < Tg; j ++) {
	      std::complex<JAN> f1 =
	             buffer -> data [(startSample + j) & bufMask];
	      std::complex<JAN> f2 =
	             buffer -> data [(startSample + Tu + j) & bufMask];
	      gamma	+= f1 * conj (f2);
	      squares	+= real (f1 * conj (f1)) + real (f2 * conj (f2));
	   }
	}
	return abs (squares - 2 * abs (gamma));
}

