/*
 * API_eSpeak.cpp
 *
 *  Created on: 05-03-2016
 *      Author: kamil
 */

#include "API_eSpeak.h"

const char *WordToString(unsigned int word)
{//========================================
// Convert a phoneme mnemonic word into a string
	int ix;
	static char buf[5];
	 
	for(ix=0; ix<3; ix++){
		buf[ix] = word >> (ix*8);
	}
	buf[4] = 0;
	return(buf);
}

// Initialize identifier
unsigned int API_eSpeak::identifier = 0;

API_eSpeak::API_eSpeak(eSpeak_Language language, string label) throw(eSpeak_exception)
{
	configuration.output = AUDIO_OUTPUT_PLAYBACK;
	configuration.buflength = 10000;
	configuration.options = espeakINITIALIZE_PHONEME_EVENTS | espeakINITIALIZE_PHONEME_IPA;
	configuration.synthFlags = espeakCHARS_AUTO;
	configuration.position_type = POS_CHARACTER;
	configuration.unique_identifier = &identifier;
	configuration.user_data = 0;

	init_eSpeak();
	set_voice(language);
	this->identifier++;
	this->user_data.audiostream = NULL;
	this->user_data.id = label;
}

API_eSpeak::API_eSpeak(eSpeak_Language language, string label, Stream* stream) throw(eSpeak_exception)
{
	configuration.output = AUDIO_OUTPUT_RETRIEVAL;
	configuration.buflength = 10000;
	configuration.options = espeakINITIALIZE_PHONEME_EVENTS | espeakINITIALIZE_PHONEME_IPA;
	configuration.synthFlags = espeakCHARS_AUTO;
	configuration.position_type = POS_CHARACTER;
	configuration.unique_identifier = &identifier;

	this->user_data.audiostream = stream;
	this->user_data.id = label;
	configuration.user_data = &user_data;

	init_eSpeak();
	set_voice(language);
	this->identifier++;
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

void API_eSpeak::set_pitch(int value)
{
	espeak_SetParameter(espeakPITCH, value, 0);
}

void API_eSpeak::set_rate(int value)
{
	double wordrate = value * (450 - 80) / 100.0 + 80.0;

	espeak_SetParameter(espeakRATE, (int)wordrate, 0);
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
	static string id;
	int margin = 300; // 300 ms at the beginning

	
	//Write phoneme transcription to stdout with timestamps
	while((type = events->type) != 0)
	{
		switch ( type )
		{
			case espeakEVENT_SAMPLERATE:
				samplerate = events->id.number;
				audiostream = ((API_eSpeak::eSpeakUserData*)(events->user_data))->audiostream;
				id = ((API_eSpeak::eSpeakUserData*)(events->user_data))->id;
				fprintf(stdout,"%s\t%4.5f\t%4.5f\t%s\n", id.c_str(),
					        0.0, 
					        0.0, "#" );
				break;
				
			case espeakEVENT_PHONEME:
				{
					fprintf(stdout,"%s\t%4.5f\t%4.5f\t%s\n", id.c_str(),
					                                         (float)(margin + previous_position)/1000, 
					                                         (float)(margin + events->audio_position)/1000, events->id.string );
					previous_position=events->audio_position;
				}
				break;
				
			case espeakEVENT_MSG_TERMINATED:
				audiostream = NULL;
				samplerate = 0;
				fprintf(stdout,"%s\t%4.5f\t%4.5f\t%s\n", id.c_str(),
					        (float)(margin + previous_position)/1000, 
					        (float)(2*margin + previous_position)/1000, "#" );
				previous_position = 0;
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
			int silencesamples = (samplerate * float(margin) / 1000);
			short* silence = new short[silencesamples];
			for ( int i = 0; i < silencesamples; i++ )
				silence[i] = 0;
			audiostream->push(silence, silencesamples);
			delete [] silence;

			audiostream->close();
			return 0;
		}
		
		//Open wave file if it is not open
		if( !audiostream->isopen() )
		{
			audiostream->init(&samplerate);
			int silencesamples = (samplerate * float(margin) / 1000);
			short* silence = new short[silencesamples];
			for ( int i = 0; i < silencesamples; i++ )
				silence[i] = 0;
			audiostream->push(silence, silencesamples);
			delete [] silence;
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
