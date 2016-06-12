/*
 * API_eSpeak.h
 *
 *  Created on: 05-03-2016
 *      Author: Kamil Sorokosz
 */

#ifndef API_ESPEAK_H_
#define API_ESPEAK_H_

#include <string>
#include <exception>
#include <cstring>
#include "speak_lib.h"
#include "AudioStream.h"

#define PLATFORM_WINDOWS
#ifdef PLATFORM_WINDOWS
#include <windows.h>
#define sleep(x) Sleep(1000*x)
#endif

using namespace std;

struct eSpeak_Configuration
{
	espeak_POSITION_TYPE position_type; /* Determines whether "position" is a number of characters, words, or sentences.*/
	espeak_AUDIO_OUTPUT output; /* the audio data can either be played by eSpeak or passed back by the SynthCallback function. */
                                    /* typedef enum {
						// PLAYBACK mode: plays the audio data, supplies events to the calling program
						AUDIO_OUTPUT_PLAYBACK,

						// RETRIEVAL mode: supplies audio data and events to the calling program
						AUDIO_OUTPUT_RETRIEVAL,

						// SYNCHRONOUS mode: as RETRIEVAL but doesn't return until synthesis is completed
						AUDIO_OUTPUT_SYNCHRONOUS,

						// Synchronous playback
						AUDIO_OUTPUT_SYNCH_PLAYBACK

					} espeak_AUDIO_OUTPUT; */
	string espeak_data_directory; /* The directory which contains the espeak-data directory, or NULL for the default location. */
	int buflength; /* The length in mS of sound buffers passed to the SynthCallback function. */
	int options; /* bit 0:  1=allow espeakEVENT_PHONEME events.
           		bit 1:  1= espeakEVENT_PHONEME events give IPA phoneme names, not eSpeak phoneme names
            		bit 15: 1=don't exit if espeak_data is not found (used for --help) */
	int synthFlags; /*  These may be OR'd together:
			      Type of character codes, one of:
				 espeakCHARS_UTF8     UTF8 encoding
				 espeakCHARS_8BIT     The 8 bit ISO-8859 character set for the particular language.
				 espeakCHARS_AUTO     8 bit or UTF8  (this is the default)
				 espeakCHARS_WCHAR    Wide characters (wchar_t)
			      espeakSSML   Elements within < > are treated as SSML elements, or if not recognised are ignored.
			      espeakPHONEMES  Text within [[ ]] is treated as phonemes codes (in espeak's Hirshenbaum encoding).
			      espeakENDPAUSE  If set then a sentence pause is added at the end of the text.  If not set then
				 this pause is suppressed. */
	unsigned int* unique_identifier; /* message identifier; helpful for identifying later data supplied to the callback. */
	void* user_data; /* pointer which will be passed to the callback function. */
	t_espeak_callback *synthCallback;
	espeak_PARAMETER* parameters; /*
				      espeakRATE:    speaking speed in word per minute.  Values 80 to 450.
				      espeakVOLUME:  volume in range 0-200 or more.
						     0=silence, 100=normal full volume,
						     greater values may produce amplitude compression or distortion
				      espeakPITCH:   base pitch, range 0-100.  50=normal
				      espeakRANGE:   pitch range, range 0-100. 0-monotone, 50=normal
				      espeakPUNCTUATION:  which punctuation characters to announce:
					 value in espeak_PUNCT_TYPE (none, all, some),
					 see espeak_GetParameter() to specify which characters are announced.
				      espeakCAPITALS: announce capital letters by:
					 0=none,
					 1=sound icon,
					 2=spelling,
					 3 or higher, by raising pitch.  This values gives the amount in Hz by which the pitch
					    of a word raised to indicate it has a capital letter.
				      espeakWORDGAP:  pause between words, units of 10mS (at the default speed)
                                     */
};

class eSpeak_exception : public exception
{
	public:
		eSpeak_exception(string error) : error(error) {}
		virtual ~eSpeak_exception() throw() {}

		virtual const char* what() const throw()
		{
			return error.c_str();
		}

	private:
		string error;
};

int eSpeak_phonemeCallback(short *wav, int numsamples, espeak_EVENT *events);

// eSpeak API
class API_eSpeak
{
	public:
		typedef enum { POLISH, ENGLISH } eSpeak_Language;
		
		// User data
		struct eSpeakUserData
		{
			string id;
			Stream* audiostream;
		};

		API_eSpeak(eSpeak_Language language, string label) throw(eSpeak_exception);
		API_eSpeak(eSpeak_Language language, string label, Stream* stream) throw(eSpeak_exception);
		virtual ~API_eSpeak();

		void set_pitch(int value);
		void set_rate(int value);
		void synthesis(const char* array, int length);
		
	protected:
		void init_eSpeak() throw(eSpeak_exception);
		void set_voice(eSpeak_Language language) throw(eSpeak_exception);
		void init_polish();
		void init_english();
		
	private:
	
		eSpeakUserData user_data;
		static unsigned int identifier;
		eSpeak_Configuration configuration;
};

#endif /* API_ESPEAK_H_ */
