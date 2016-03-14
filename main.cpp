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

int main()
{
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
		
		eSpeak->synthesis(text.c_str(), text.length() + 1);
		
		if ( eSpeak )
			delete eSpeak;
		
		if ( audiostream )
			delete audiostream;
	}

	return 0;
}
