#include "API_eSpeak.h"
using namespace std;

int main()
{
	string text = "Cześć to działa";
	API_eSpeak eSpeak = API_eSpeak(API_eSpeak::POLISH);

	char memory[30]; // i don't know why this is important!
	eSpeak.synthesis(text.c_str(), text.length() + 1);

	return 0;
}
