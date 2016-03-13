#include "API_eSpeak.h"
#include <string>
#include <iostream>
#include <iterator>
using namespace std;

class line : public string {};

istream &operator>>(istream &is, line &l)
{
    getline(is, l);
    return is;
}

int main()
{
	API_eSpeak eSpeak = API_eSpeak(API_eSpeak::POLISH);
	istream_iterator<line> begin(cin);
	istream_iterator<line> end;
   
    for (istream_iterator<line> it = begin; it != end; it++)
    {
      	string line = (*it);
 		eSpeak.synthesis(line.c_str(), line.length() + 1);
	}
    
	return 0;
}
