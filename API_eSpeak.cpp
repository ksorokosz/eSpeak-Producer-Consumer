/*
 * API_eSpeak.cpp
 *
 *  Created on: 05-03-2016
 *      Author: kamil
 */

#include "API_eSpeak.h"

API_eSpeak::API_eSpeak(eSpeak_Language language) throw(eSpeak_exception)
{
	configuration.output = AUDIO_OUTPUT_PLAYBACK;
	configuration.buflength = 500;
	configuration.options = espeakINITIALIZE_PHONEME_EVENTS | espeakINITIALIZE_PHONEME_IPA;
	configuration.synthFlags = espeakCHARS_AUTO;

	init_eSpeak();
	set_voice(language);
}

API_eSpeak::API_eSpeak(eSpeak_Language language, eSpeak_Configuration& configuration)
                throw(eSpeak_exception)
{
	this->configuration = configuration;

	init_eSpeak();
	set_voice(language);
}

API_eSpeak::~API_eSpeak()
{

}

void API_eSpeak::synthesis(const char* array, int length)
{
		espeak_Synth( array, length, 0, configuration.position_type, 0, configuration.synthFlags,
		              configuration.unique_identifier, configuration.user_data );
		espeak_Synchronize();
}

void API_eSpeak::init_eSpeak()  throw(eSpeak_exception)
{

	espeak_Initialize(configuration.output, configuration.buflength,
			          NULL, configuration.options );

	espeak_SetSynthCallback(eSpeak_phonemeCallback);
}

void API_eSpeak::set_voice(eSpeak_Language language)  throw(eSpeak_exception)
{
	switch ( language )
	{
	case POLISH:
		init_polish(); break;
	case ENGLISH:
		init_english(); break;
	default:
		throw eSpeak_exception("Language not supported");
	}
}

void API_eSpeak::init_polish()
{
	espeak_SetVoiceByName("pl");
}

void API_eSpeak::init_english()
{
	espeak_SetVoiceByName("en");

}

const char *PhonemeToString(unsigned int word)
{
    // Convert a phoneme mnemonic word into a string
	int ix;
	static char buf[5];

	for(ix=0; ix<3; ix++){
		buf[ix] = word >> (ix*8);
	}
	buf[4] = 0;
	return(buf);
}

/* Phoneme Callback */
int eSpeak_phonemeCallback(short *wav, int numsamples, espeak_EVENT *events)
{//====================================================================
	int type;
	static int previous_position = 0; // previous position declared as static

	//Write phoneme transcription to stdout with timestamps
	while((type = events->type) != 0)
	{
		switch ( type )
		{
			case espeakEVENT_PHONEME:
				{
					const char *word = PhonemeToString(events->id.number);
					fprintf(stdout,"%4.3f\t%4.3f\t%s\n", (float)previous_position/1000, (float)events->audio_position/1000,word );
					previous_position=events->audio_position;
				}
				break;

			default:
				break;
		}
		events++;
	}

	return(0);
}

