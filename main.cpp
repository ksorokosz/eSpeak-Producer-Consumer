#include "API_eSpeak.h"
using namespace std;

int main()
{
	string text = "tralalala";
	API_eSpeak eSpeak = API_eSpeak(API_eSpeak::POLISH);

	eSpeak.synthesis(text.c_str(), text.length() + 1);

	return 0;
}
