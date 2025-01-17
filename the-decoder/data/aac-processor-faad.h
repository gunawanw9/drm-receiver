#
/*
 *    Copyright (C) 2020
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair Computing
 *
 *    This file is part of the SDRunoPlugin_drm
 *
 *    drm plugin is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    drm plugin is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with drm plugin; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#
#ifndef	__AAC_PROCESSOR_FAAD_H
#define	__AAC_PROCESSOR_FAAD_H

#include	<QObject>
#include	<cstring>
#include	"neaacdec.h"
#include	"up-filter.h"
#include	"ringbuffer.h"

class	stateDescriptor;
class	drmDecoder;

class	aacProcessor_faad: public QObject {
Q_OBJECT
public:
		aacProcessor_faad	(stateDescriptor *,
	                                 drmDecoder *,
	                                 RingBuffer<std::complex<float>> *);
		~aacProcessor_faad	();
	void	process_aac	(uint8_t *, int16_t,
	                         int16_t, int16_t, int16_t, int16_t);
private:
	stateDescriptor	*theState;
	drmDecoder	*m_form;
	RingBuffer<std::complex<float>> *audioOut;
	upFilter	upFilter_24000;
	upFilter	upFilter_12000;
	int16_t		numFrames;
	void	handle_uep_audio	(uint8_t *, int16_t,
	                         int16_t, int16_t, int16_t, int16_t);
	void	handle_eep_audio	(uint8_t *, int16_t, int16_t, int16_t);
	void    writeOut        (int16_t *, int16_t, int32_t);
        void    toOutput	(std::complex<float> *, int16_t);
        void    playOut         (int16_t);
	bool    checkfor        (uint8_t, bool, uint8_t);
        bool    initDecoder     (int16_t, bool, uint8_t);
        void    decodeFrame     (uint8_t *,
                                 int16_t,
                                 bool *,
                                 int16_t        *,              // buffer
                                 int16_t        *, int32_t *);
        void    closeDecoder    (void);
	NeAACDecHandle  theDecoder;
        bool            SBR_flag;
        uint8_t         audioMode;
        int16_t         audioRate;
	int		goodFrames;
	int		badFrames;
signals:
	void		set_faadSyncLabel (bool);
	void		set_aacDataLabel (const QString &);
	void		audioAvailable	();
};

#endif

