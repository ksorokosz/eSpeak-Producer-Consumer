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
		string file;
		while(getline(iss, token, '\t')) 
			tokens.push_back(token);
	
		string text = tokens[0];
		if ( tokens.size() > 1 )
			file = tokens[1];

		// Synthesis
		API_eSpeak* eSpeak = NULL; 
		
		if ( file.length() )
			eSpeak = new API_eSpeak(API_eSpeak::POLISH, file);
		else
			eSpeak = new API_eSpeak(API_eSpeak::POLISH);
		
		eSpeak->synthesis(text.c_str(), text.length() + 1);
		
		if ( eSpeak )
			delete eSpeak;
	}

	return 0;
}
