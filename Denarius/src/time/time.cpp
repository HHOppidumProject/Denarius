#include "time.h"

std::string dtime::currentTime()
{
	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];

	time(&rawtime);
	strftime(buffer, 80, "%m/%d/%Y, %H:%M:%S", localtime(&rawtime));
	return buffer;
}