######################################################################
# Automatically generated by qmake (2.01a) Wed Jun 19 18:47:11 2013
######################################################################
TEMPLATE = app
QT	+= widgets xml
CONFIG	-= console
TARGET	= drm-radio
QMAKE_CFLAGS	+= -flto -ffast-math 
QMAKE_CXXFLAGS	+= -flto -ffast-math 
QMAKE_LFLAGS	+= -flto
#QMAKE_CFLAGS	+= -g
#QMAKE_CXXFLAGS	+= -g
#QMAKE_LFLAGS	+= -g
#QMAKE_CXXFLAGS	+= -fsanitize=address
#QMAKE_CFLAGS	+= -fsanitize=address
#QMAKE_LFLAGS	+= -fsanitize=address
RC_ICONS        =  swradio.ico
RESOURCES       += resources.qrc

DEPENDPATH += .  \
	      decimators \
	      filters \
	      various \
	      output \
	      scopes-qwt6 \
	      devices \
	      devices/filereader \
	      decoders  \
	      ./decoders/drm-decoder/ \
	      ./decoders/drm-decoder/ofdm \
	      ./decoders/drm-decoder/signal-handling \
              ./decoders/drm-decoder/signal-handling/eq-1 \
              ./decoders/drm-decoder/support \
              ./decoders/drm-decoder/parameters \
              ./decoders/drm-decoder/msc \
	      ./decoders/drm-decoder/sdc \
	      ./decoders/drm-decoder/fac \
              ./decoders/drm-decoder/data 

INCLUDEPATH += .  \
	      decimators \
	      filters \
	      various \
	      output \
	      scopes-qwt6 \
	      devices \
	      devices/filereader \
	      decoders \
	      ./decoders/drm-decoder/ \
	      ./decoders/drm-decoder/ofdm \
	      ./decoders/drm-decoder/signal-handling \
              ./decoders/drm-decoder/signal-handling/eq-1 \
              ./decoders/drm-decoder/support \
              ./decoders/drm-decoder/parameters \
              ./decoders/drm-decoder/msc \
	      ./decoders/drm-decoder/sdc \
	      ./decoders/drm-decoder/fac \
	      ./decoders/drm-decoder/data 

#CONFIG += static

# Input
HEADERS += ./radio-constants.h \
	   ./radio.h \
	   ./bandplan.h \
           ./output/upconverter.h \
	   ./output/audiosink.h \
           ./various/agchandler.h \
           ./various/ringbuffer.h \
           ./various/fft.h \
	   ./various/shifter.h \
	   ./various/utilities.h \
	   ./various/slidingfft.h \
	   ./various/program-list.h \
	   ./various/popup-keypad.h \
	   ./various/s-meter.h \
	   ./various/downconverter.h \
	   ./decimators/newconverter.h \
	   ./decimators/decimator.h \
           ./filters/fft-filters.h \
           ./filters/fir-filters.h \
           ./filters/iir-filters.h \
	   ./filters/decimating_filter.h \
	   ./scopes-qwt6/virtual-scope.h \
	   ./scopes-qwt6/spectrogramdata.h \
	   ./scopes-qwt6/waterfall-scope.h \
	   ./scopes-qwt6/spectrum-scope.h \
           ./scopes-qwt6/scope.h \
           ./scopes-qwt6/fft-scope.h \
	   ./devices/device-handler.h \
           ./devices/filereader/filereader.h \
           ./devices/filereader/filehulp.h \
           ./decoders/virtual-decoder.h \
	   ./decoders/drm-decoder/drm-decoder.h  \
	   ./decoders/drm-decoder/iqdisplay.h \
	   ./decoders/drm-decoder/eqdisplay.h \
	   ./decoders/drm-decoder/basics.h \
	   ./decoders/drm-decoder/frame-processor.h \
	   ./decoders/drm-decoder/ofdm/reader.h \
	   ./decoders/drm-decoder/ofdm/correlator.h \
	   ./decoders/drm-decoder/ofdm/timesync.h \
	   ./decoders/drm-decoder/ofdm/freqsyncer.h \
	   ./decoders/drm-decoder/ofdm/word-collector.h \
	   ./decoders/drm-decoder/signal-handling/referenceframe.h \
	   ./decoders/drm-decoder/signal-handling/equalizer-base.h \
	   ./decoders/drm-decoder/signal-handling/eq-1/equalizer-1.h \
	   ./decoders/drm-decoder/signal-handling/eq-1/estimator-base.h \
	   ./decoders/drm-decoder/signal-handling/eq-1/estimator-2.h \
	   ./decoders/drm-decoder/support/my-array.h \
	   ./decoders/drm-decoder/support/viterbi-drm.h \
	   ./decoders/drm-decoder/support/mapper.h \
	   ./decoders/drm-decoder/support/prbs.h \
	   ./decoders/drm-decoder/support/checkcrc.h \
	   ./decoders/drm-decoder/support/qam64-metrics.h \
	   ./decoders/drm-decoder/support/qam16-metrics.h \
	   ./decoders/drm-decoder/support/qam4-metrics.h \
	   ./decoders/drm-decoder/support/deinterleaver.h \
	   ./decoders/drm-decoder/support/matrix2.h \
	   ./decoders/drm-decoder/support/protlevels.h \
	   ./decoders/drm-decoder/support/mer4-values.h \
	   ./decoders/drm-decoder/support/mer16-values.h \
	   ./decoders/drm-decoder/support/mer64-values.h \
	   ./decoders/drm-decoder/support/fft-complex.h \
	   ./decoders/drm-decoder/support/drm-shifter.h \
	   ./decoders/drm-decoder/parameters/state-descriptor.h \
	   ./decoders/drm-decoder/parameters/puncture-tables.h \
	   ./decoders/drm-decoder/fac/fac-tables.h \
	   ./decoders/drm-decoder/fac/fac-processor.h \
	   ./decoders/drm-decoder/sdc/sdc-streamer.h \
	   ./decoders/drm-decoder/sdc/sdc-processor.h \
	   ./decoders/drm-decoder/msc/backend-controller.h \
	   ./decoders/drm-decoder/msc/msc-processor.h \
	   ./decoders/drm-decoder/msc/msc-handler-qam16.h \
	   ./decoders/drm-decoder/msc/msc-handler-qam64.h \
	   ./decoders/drm-decoder/msc/msc-streamer.h \
	   ./decoders/drm-decoder/msc/msc-handler.h \
	   ./decoders/drm-decoder/data/data-processor.h \
	   ./decoders/drm-decoder/data/up-filter.h \
	   ./decoders/drm-decoder/data/fec-handler.h \
	   ./decoders/drm-decoder/data/galois.h \
	   ./decoders/drm-decoder/data/reed-solomon.h \
	   ./decoders/drm-decoder/data/message-processor.h \
	   ./decoders/drm-decoder/data/packet-assembler.h \
	   ./decoders/drm-decoder/data/virtual-datahandler.h \
	   ./decoders/drm-decoder/data/mot-data.h 

FORMS	+= ./newradio.ui \
	   ./devices/filereader/filereader-widget.ui
FORMS	+= ./decoders/drm-decoder/drmdecoder.ui

SOURCES += ./main.cpp \
	   ./radio.cpp \
	   ./bandplan.cpp \
	   ./output/upconverter.cpp \
           ./output/audiosink.cpp \
           ./various/agchandler.cpp \
           ./various/fft.cpp \
	   ./various/popup-keypad.cpp \
	   ./various/s-meter.cpp \
           ./various/shifter.cpp \
	   ./various/utilities.cpp \
	   ./various/slidingfft.cpp \
	   ./various/program-list.cpp \
	   ./various/downconverter.cpp \
           ./filters/fft-filters.cpp \
           ./filters/fir-filters.cpp \
           ./filters/iir-filters.cpp \
	   ./filters/decimating_filter.cpp \
	   ./scopes-qwt6/virtual-scope.cpp \
	   ./scopes-qwt6/waterfall-scope.cpp \
	   ./scopes-qwt6/spectrum-scope.cpp \
           ./scopes-qwt6/scope.cpp \
           ./scopes-qwt6/fft-scope.cpp \
	   ./decimators/decimator.cpp \
	   ./devices/device-handler.cpp \
           ./devices/filereader/filereader.cpp \
           ./devices/filereader/filehulp.cpp \
           ./decoders/virtual-decoder.cpp  \
	   ./decoders/drm-decoder/drm-decoder.cpp  \
	   ./decoders/drm-decoder/eqdisplay.cpp \
	   ./decoders/drm-decoder/iqdisplay.cpp \
	   ./decoders/drm-decoder/basics.cpp \
	   ./decoders/drm-decoder/frame-processor.cpp \
	   ./decoders/drm-decoder/ofdm/reader.cpp \
	   ./decoders/drm-decoder/ofdm/correlator.cpp \
	   ./decoders/drm-decoder/ofdm/timesync.cpp \
	   ./decoders/drm-decoder/ofdm/freqsyncer.cpp \
	   ./decoders/drm-decoder/ofdm/word-collector.cpp \
	   ./decoders/drm-decoder/signal-handling/referenceframe.cpp \
	   ./decoders/drm-decoder/signal-handling/equalizer-base.cpp \
	   ./decoders/drm-decoder/signal-handling/eq-1/equalizer-1.cpp \
	   ./decoders/drm-decoder/signal-handling/eq-1/estimator-base.cpp \
	   ./decoders/drm-decoder/signal-handling/eq-1/estimator-1.cpp \
	   ./decoders/drm-decoder/support/viterbi-drm.cpp \
	   ./decoders/drm-decoder/support/mapper.cpp \
	   ./decoders/drm-decoder/support/prbs.cpp \
	   ./decoders/drm-decoder/support/checkcrc.cpp \
	   ./decoders/drm-decoder/support/qam64-metrics.cpp \
	   ./decoders/drm-decoder/support/qam16-metrics.cpp \
	   ./decoders/drm-decoder/support/qam4-metrics.cpp \
 	   ./decoders/drm-decoder/support/deinterleaver.cpp \
	   ./decoders/drm-decoder/support/matrix2.cpp \
	   ./decoders/drm-decoder/support/protlevels.cpp \
	   ./decoders/drm-decoder/support/mer4-values.cpp \
	   ./decoders/drm-decoder/support/mer16-values.cpp \
	   ./decoders/drm-decoder/support/mer64-values.cpp \
	   ./decoders/drm-decoder/support/fft-complex.cpp \
	   ./decoders/drm-decoder/support/drm-shifter.cpp \
	   ./decoders/drm-decoder/parameters/state-descriptor.cpp \
	   ./decoders/drm-decoder/parameters/puncture-tables.cpp \
	   ./decoders/drm-decoder/fac/fac-tables.cpp \
	   ./decoders/drm-decoder/fac/fac-processor.cpp \
	   ./decoders/drm-decoder/sdc/sdc-streamer.cpp \
	   ./decoders/drm-decoder/sdc/sdc-processor.cpp \
	   ./decoders/drm-decoder/msc/backend-controller.cpp \
	   ./decoders/drm-decoder/msc/msc-processor.cpp \
	   ./decoders/drm-decoder/msc/msc-handler-qam16.cpp \
	   ./decoders/drm-decoder/msc/msc-handler-qam64.cpp \
	   ./decoders/drm-decoder/msc/msc-streamer.cpp \
	   ./decoders/drm-decoder/msc/msc-handler.cpp \
	   ./decoders/drm-decoder/data/data-processor.cpp \
	   ./decoders/drm-decoder/data/up-filter.cpp \
	   ./decoders/drm-decoder/data/fec-handler.cpp \
	   ./decoders/drm-decoder/data/galois.cpp \
	   ./decoders/drm-decoder/data/reed-solomon.cpp \
	   ./decoders/drm-decoder/data/message-processor.cpp \
	   ./decoders/drm-decoder/data/packet-assembler.cpp \
	   ./decoders/drm-decoder/data/virtual-datahandler.cpp \
	   ./decoders/drm-decoder/data/mot-data.cpp 

unix {
exists ("./.git") {
   GITHASHSTRING = $$system(git rev-parse --short HEAD)
   !isEmpty(GITHASHSTRING) {
       message("Current git hash = $$GITHASHSTRING")
       DEFINES += GITHASH=\\\"$$GITHASHSTRING\\\"
   }
}
isEmpty(GITHASHSTRING) {
    DEFINES += GITHASH=\\\"------\\\"
}

DESTDIR		= ./linux-bin
CONFIG		+= sdrplay
DEFINES		+= HAVE_DRM_DECODER
DEFINES		+= ESTIMATOR_1
CONFIG		+= faad
#CONFIG		+= fdk-aac

LIBS		+= -L/usr/lib64
LIBS		+= -L/lib64
INCLUDEPATH	+= /usr/include/qt5/qwt
LIBS		+= -lqwt-qt5 -lrt -lsndfile -lsamplerate -lportaudio -lusb-1.0 -lfftw3f -ldl
}

win32 {
exists ("./.git") {
   GITHASHSTRING = $$system(git rev-parse --short HEAD)
   !isEmpty(GITHASHSTRING) {
       message("Current git hash = $$GITHASHSTRING")
       DEFINES += GITHASH=\\\"$$GITHASHSTRING\\\"
   }
}
isEmpty(GITHASHSTRING) {
    DEFINES += GITHASH=\\\"------\\\"
}

DESTDIR		= ../../windows-swradio
CONFIG		+= sdrplay
CONFIG		+= hackrf
CONFIG		+= rtlsdr
# includes in mingw differ from the includes in fedora linux
INCLUDEPATH += /usr/i686-w64-mingw32/sys-root/mingw/include
INCLUDEPATH += /usr/i686-w64-mingw32/sys-root/mingw/include/qt5/qwt
INCLUDEPATH += C:\msys32\mingw32\include\qwt
LIBS    += -L/usr/i686-w64-mingw32/sys-root/mingw/lib
LIBS    += -lfftw3f
LIBS    += -lportaudio
LIBS    += -lqwt-qt5
#LIBS    += -lqwt
LIBS    += -lusb-1.0
LIBS    += -lsndfile
LIBS    += -lsamplerate
LIBS    += -lole32
#LIBS	+= -lfaad_drm
LIBS    += -lwinmm
}

#       the SDRplay
#
sdrplay {
        DEFINES         += HAVE_SDRPLAY
        FORMS           += ./devices/sdrplay-handler/sdrplay-widget.ui
        DEPENDPATH     += ./devices/sdrplay-handler
        INCLUDEPATH     += ./devices/sdrplay-handler
        HEADERS         += ./devices/sdrplay-handler/sdrplay-handler.h \
                           ./devices/sdrplay-handler/sdrplayselect.h
        SOURCES         += ./devices/sdrplay-handler/sdrplay-handler.cpp \
                           ./devices/sdrplay-handler/sdrplayselect.cpp
}

hackrf	{
	DEFINES		+= HAVE_HACKRF
        FORMS           += ./devices/hackrf-handler/hackrf-widget.ui
        DEPENDPATH	+= ./devices/hackrf-handler
        INCLUDEPATH	+= ./devices/hackrf-handler
        HEADERS         += ./devices/hackrf-handler/hackrf-handler.h 
        SOURCES         += ./devices/hackrf-handler/hackrf-handler.cpp 
}

rtlsdr	{
	DEFINES		+= HAVE_RTLSDR
        FORMS           += ./devices/rtlsdr-handler/rtlsdr-widget.ui
        DEPENDPATH	+= ./devices/rtlsdr-handler
        INCLUDEPATH	+= ./devices/rtlsdr-handler
        HEADERS         += ./devices/rtlsdr-handler/rtlsdr-handler.h  \
	                   ./devices/rtlsdr-handler/dongleselect.h
        SOURCES         += ./devices/rtlsdr-handler/rtlsdr-handler.cpp \
	                   ./devices/rtlsdr-handler/dongleselect.cpp
}

pmsdr	{
	DEFINES		+= HAVE_PMSDR
	TARGET		= swradio-pmsdr
	FORMS		+= ./devices/pmsdr-handler/pmsdr-widget.ui
	DEPENDPATH	+= ./devices/pmsdr-handler
        INCLUDEPATH     += ./devices/pmsdr-handler
        HEADERS         += ./devices/pmsdr-handler/pmsdr-handler.h \
			   ./devices/pmsdr-handler/pmsdr_comm.h \
	                   ./devices/pmsdr-handler/pmsdr_usb.h \
	                   ./devices/pmsdr-handler/si570-handler.h 
        SOURCES         += ./devices/pmsdr-handler/pmsdr-handler.cpp \
			   ./devices/pmsdr-handler/pmsdr_comm.cpp \
	                   ./devices/pmsdr-handler/pmsdr_usb.cpp \
	                   ./devices/pmsdr-handler/si570-handler.cpp 
	CONFIG		+= cardreader
}

extio	{
	DEFINES		+= HAVE_EXTIO
	TARGET		= swradio-extio
	FORMS		+= ./devices/extio-handler/extio-widget.ui
	DEPENDPATH	+= ./devices/extio-handler
        INCLUDEPATH     += ./devices/extio-handler
        HEADERS         += ./devices/extio-handler/extio-handler.h 
        SOURCES         += ./devices/extio-handler/extio-handler.cpp 
	CONFIG		+= cardreader
}

fdk-aac	{
DEFINES		+= __WITH_FDK_AAC__
LIBS		+= -lfdk-aac
#LIBS		+= -lfdk-aac -larmadillo
INCLUDEPATH	+= /usr/local/include/fdk-aac
HEADERS	+= ./decoders/drm-decoder/data/aac-processor-fdk.h \
	           ./decoders/drm-decoder/data/xheaac-processor.h 
SOURCES		+= ./decoders/drm-decoder/data/aac-processor-fdk.cpp \
	           ./decoders/drm-decoder/data/xheaac-processor.cpp 
}

faad	{
DEFINES		+= __WITH_FAAD__
HEADERS		+=  ./decoders/drm-decoder/data/aac-processor-faad.h  \
	            ./decoders/drm-decoder/data/drm-aacdecoder.h \
	            ./decoders/drm-decoder/data/neaacdec_dll.h 
SOURCES		+= ./decoders/drm-decoder/data/aac-processor-faad.cpp  \
	           ./decoders/drm-decoder/data/drm-aacdecoder.cpp 
LIBS		+= -lfaad_drm
#LIBS		+= -lfaad_drm -larmadillo
}
