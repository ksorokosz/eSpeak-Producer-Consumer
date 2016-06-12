#include "API_eSpeak.h"
#include <string>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>
using namespace std;

class line : public string {};

istream &operator>>(istream &is, line &l)
{
    getline(is, l);
    return is;
}

// -p <pitch-value>
int main(int argc, char * argv[])
{
	int pitch_value = 50;
	int rate_value = 50;
	
	if ( argc > 1 )
	{
		for ( int i = 1; i < argc; i++ )
		{
			if ( string(argv[i]) == "-p" )
			{
				if ( argc < i+2 ) // i + 2 because argc = number of used switches + 1
				{
						fprintf(stderr, "Please provide pitch value\n");
						exit(1);
				}
				
				pitch_value = atoi(argv[++i]);
			}
			else if ( string(argv[i]) == "-r" )
			{
				if ( argc < i+2 ) // i + 2 because argc = number of used switches + 1
				{
						fprintf(stderr, "Please provide pitch value\n");
						exit(1);
				}
				
				rate_value = atoi(argv[++i]);
			}
			else
			{
				fprintf(stderr,"Unknown switch\n");
				exit(1);
			}
		}
	}
		
	istream_iterator<line> begin(cin);
	istream_iterator<line> end;

	for (istream_iterator<line> it = begin; it != end; it++)
	{
		string line = (*it);
		istringstream iss(line);
		vector<string> tokens;
		
		string token;
		string text, label, file;
		while(getline(iss, token, '\t')) 
			tokens.push_back(token);
	
		label = tokens[0]; // always first
		text = tokens[tokens.size() - 1]; // always last
		if ( tokens.size() > 2 )
		{
			file = tokens[1];
		}
		
		// Synthesis
		API_eSpeak* eSpeak = NULL; 
		Stream* audiostream = NULL;
		
		if ( file.length() )
		{
			audiostream = new AudioStream(file);
			eSpeak = new API_eSpeak(API_eSpeak::POLISH, label, audiostream);
		}
		else
			eSpeak = new API_eSpeak(API_eSpeak::POLISH, label);
		
		eSpeak->set_pitch(pitch_value);
		eSpeak->set_rate(rate_value);
		eSpeak->synthesis(text.c_str(), text.length() + 1);
		
		if ( eSpeak )
			delete eSpeak;
		
		if ( audiostream )
			delete audiostream;
	}

	return 0;
}
