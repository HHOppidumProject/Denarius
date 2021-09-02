#include "time.h"

std::string dtime::currentTime()
{
	//std::chrono::high_resolution_clock Clock;
	//auto t = Clock.now();
	//std::cout << t << std::endl;
	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];

	time(&rawtime);
	strftime(buffer, 80, "%d/%m/%Y, %H:%M:%S", localtime(&rawtime));
	return buffer;
}