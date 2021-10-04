

-----------------------------------------------------------------------------
Redesign and re-implementation of the drm decoder
-----------------------------------------------------------------------------

DRM (Digital Radio Mondiale) is - as the name suggests - a form of digital
radio. 
In Europe drm is not very popular, most transmissions that started
in the first decade of the century already stopped. However,
Rumenia still has a regular DRM transmission.

![overview](/drm-receiver.png?raw=true)

The drm receiver is an experimental tool, a heavily reduced swradio,
with a single decoder, just for drm.

![overview](/drm-decoder.png?raw=true)

The picture of the decoder gives quite some information about the received
and decoded signal.

 * at the top left, the measured frequency offset, two numbers, one for the so-called coarse offset, i.e. the number of carriers off (here -281 Hz), the second number telling the fine offset, i.e. the offset in the carrier (here -21.3 Hz)

 * the number 12:51 gives the time (UTC) derived from the transmission

 * 24000 stereo is the indiction of the rate of the audio output

 * it can be seen that there is one service being transmitted (the software is able to hanfle 2 audio transmissions), here Radio kuwait

 * the numbers 544 -2 give an indication of the time offset, here it states that after 544 "words" (each 320 samples), a correction was needed with at most
 two samples

 * the number 0.986 tells that from the 1000 audio frames, 986 were correct

 * the labels time sync etc speak for themselves

Below there is a row with 3, B, QAM64, AAC, which tells that the spectrum
is of type 3 (which just means 1 10 KHz spectrum), the mode is Mode B, the
bits carrying the audio are encoded as QAM 64, and the audio is encoded as AAC.
The empty box then is available for text messages.

Below this, there are 4 numbers, giving some information on the quality of
the signal

 * the number 68.59 tells that the quality of the signal in which the FAC is encoded is very good (FAC data is encoded as QAM4);

 * the number 17.52 tells that the quality of the signal in which the SDC is
encoded is good (SDC data is encoded as QAM16);

 * the number 8.215 tells that the quality of the signal in which the audio data is encoded is fairly good (MSC data is encoded as QAM64);

The scopes at the bottom give some visual information about the restored signal.
The scope left tells the correction that is to be applied to the data
in the frequency domain, the red line tells the correction on the amplitude,
the blue line the correction on the phase. It shows that the signal here
is fairly reasonable.

The scope right gives the constellation of the restored signal. Ideally
there are 64 dots evenly distributed over the screen.


----------------------------------------------------------------------------
----------------------------------------------------------------------------

A note on the implementation

The implementation is derived from the implementation of the drm decoder
at the sw radio. The software was restructured though and limited in functionality: this software is restricted to handling audio streams.

----------------------------------------------------------------------------
----------------------------------------------------------------------------

For windows an installer is available
For Linux, one has to create an executable. Note that to keep things simple
the only path is using qmake/make.

By setting some parameters in the ".pro" file, one may choose between
aac decoding using the "faad_drm" library or the "fdk-aac" library.

The latter has the (potential) advantage of being able to decode xHE-AAC
as well

