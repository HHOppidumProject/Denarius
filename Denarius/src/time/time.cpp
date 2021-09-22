#include "time.h"

std::string dtime::currentTime()
{

	using namespace std::chrono;
	
	auto temp = system_clock::to_time_t(system_clock::now());
	std::tm* tt = std::localtime(&(temp));
	tt->tm_sec = 0;
	
	char buffer[80];
	strftime(buffer, 80, "%d/%m/%Y, %H:%M", tt);

	auto minute = system_clock::from_time_t(std::mktime(tt)).time_since_epoch();

	auto now = time_point_cast<nanoseconds>(system_clock::now()).time_since_epoch();
	std::string res = buffer;
	res += ":" + std::to_string(duration_cast<seconds>(now).count() - duration_cast<seconds>(minute).count()) +
		":" + std::to_string(duration_cast<milliseconds>(now).count() - duration_cast<milliseconds>(minute).count()).substr(2, 3) +
		":" + std::to_string(duration_cast<microseconds>(now).count() - duration_cast<microseconds>(minute).count()).substr(5, 3) +
		":" + std::to_string(duration_cast<nanoseconds>(now).count() - duration_cast<nanoseconds>(minute).count()).substr(8, 3);

	return res;
}