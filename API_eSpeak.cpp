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
	configuration.buflength = 10000;
	configuration.options = espeakINITIALIZE_PHONEME_EVENTS;
	configuration.synthFlags = espeakCHARS_AUTO;
	configuration.position_type = POS_CHARACTER;
	configuration.unique_identifier = 0;
	configuration.user_data = 0;

	init_eSpeak();
	set_voice(language);
}

API_eSpeak::API_eSpeak(eSpeak_Language language, string wavepath) throw(eSpeak_exception)
{
	configuration.output = AUDIO_OUTPUT_RETRIEVAL;
	configuration.buflength = 10000;
	configuration.options = espeakINITIALIZE_PHONEME_EVENTS;
	configuration.synthFlags = espeakCHARS_AUTO;
	configuration.position_type = POS_CHARACTER;
	configuration.unique_identifier = 0;
	configuration.user_data = new AudioStream(wavepath);

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
	if ( configuration.user_data != NULL )
	{
		delete (Stream*)configuration.user_data;
	}
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
			          ".", configuration.options );

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

/* Phoneme Callback */
int eSpeak_phonemeCallback(short *wav, int numsamples, espeak_EVENT *events)
{//====================================================================
	int type;
	static int previous_position = 0; // previous position declared as static
	static int samplerate = 0;
	static Stream* audiostream = NULL;
	
	//Write phoneme transcription to stdout with timestamps
	while((type = events->type) != 0)
	{
		switch ( type )
		{
			case espeakEVENT_SAMPLERATE:
				samplerate = events->id.number;
				audiostream = (Stream*)(events->user_data);
				break;
				
			case espeakEVENT_PHONEME:
				{
					fprintf(stdout,"%4.3f\t%4.3f\t%s\n", (float)previous_position/1000, (float)events->audio_position/1000, events->id.string );
					previous_position=events->audio_position;
				}
				break;

			default:
				break;
		}
		events++;
	}

	try
	{
		if ( audiostream == NULL )
			return 0;
		
		if(wav == NULL)
		{			
				audiostream->close();
				return 0;
		}
		
		//Open wave file if it is not open
		if( !audiostream->isopen() )
		{
			audiostream->init(&samplerate);
		}
		
		//Write wav file
		if(numsamples > 0)
		{
			audiostream->push(wav, numsamples);
		}
	}
	catch ( exception& e)
	{
		e.what();
		return 1;
	}

	return(0);
}