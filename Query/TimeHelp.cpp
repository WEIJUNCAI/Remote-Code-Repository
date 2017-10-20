/////////////////////////////////////////////////////////////////////////////
//  TimeHelp.cpp- utility class for Query class                            //
//  Language:     C++, VS 2015                                             //
//  Platform:     SurfaceBook, Windows 10 Pro                              //
//  Application:  Project1 for CSE687 - Object Oriented Design             //
//  Author:       Weijun Cai                                               //
/////////////////////////////////////////////////////////////////////////////


#include <sstream>
#include <iostream>
#include "TimeHelp.h"

bool TimeHelp::if_Succeed(const std::string time1, const std::string time2)
{
	tm tm1 = StringToTm(time1);
	tm tm2 = StringToTm(time2);

	if (tm1.tm_year != tm2.tm_year)
		return tm1.tm_year > tm2.tm_year;
	if (tm1.tm_mon != tm2.tm_mon)
		return tm1.tm_mon > tm2.tm_mon;
	if (tm1.tm_mday != tm2.tm_mday)
		return tm1.tm_mday > tm2.tm_mday;
	if (tm1.tm_hour != tm2.tm_hour)
		return tm1.tm_hour > tm2.tm_hour;
	if (tm1.tm_min != tm2.tm_min)
		return tm1.tm_min > tm2.tm_min;
	if (tm1.tm_sec != tm2.tm_sec)
		return tm1.tm_sec > tm2.tm_sec;
	return true;
}

bool TimeHelp::if_Precede(const std::string time1, const std::string time2)
{
	tm tm1 = TimeHelp::StringToTm(time1);
	tm tm2 = TimeHelp::StringToTm(time2);

	if (tm1.tm_year != tm2.tm_year)
		return tm1.tm_year < tm2.tm_year;
	if (tm1.tm_mon != tm2.tm_mon)
		return tm1.tm_mon < tm2.tm_mon;
	if (tm1.tm_mday != tm2.tm_mday)
		return tm1.tm_mday < tm2.tm_mday;
	if (tm1.tm_hour != tm2.tm_hour)
		return tm1.tm_hour < tm2.tm_hour;
	if (tm1.tm_min != tm2.tm_min)
		return tm1.tm_min < tm2.tm_min;
	if (tm1.tm_sec != tm2.tm_sec)
		return tm1.tm_sec < tm2.tm_sec;
	return true;
}

tm TimeHelp::StringToTm(const std::string time)
{
	tm result = tm();
	std::string timePart;
	std::istringstream iss(time);

	iss >> timePart;
	result.tm_wday = week[timePart];
	iss >> timePart;
	result.tm_mon = month[timePart];
	iss >> timePart;
	if (timePart.size() == 2)
		result.tm_mday = (timePart[0] - '0') * 10 + (timePart[1] - '0');
	else
		result.tm_mday = timePart[0] - '0';
	iss >> timePart;
	result.tm_hour = (timePart[0] - '0') * 10 + (timePart[1] - '0');
	result.tm_min = (timePart[3] - '0') * 10 + (timePart[4] - '0');
	result.tm_sec = (timePart[6] - '0') * 10 + (timePart[7] - '0');
	iss >> timePart;
	for (int i = 0; i < 4; ++i)
	{
		result.tm_year = result.tm_year * 10 + timePart[i] - '0';
	}

	return result;
}


// test stub
#ifdef TIME_HELP_CPP
void main()
{
	tm test = TimeHelp::StringToTm("Sat Feb  4 17:10:08 2017");
	std::cout << test.tm_wday << std::endl;
	std::cout << test.tm_mday << std::endl;
	std::cout << test.tm_mon << std::endl;
	std::cout << test.tm_hour << std::endl;
	std::cout << test.tm_min << std::endl;
	std::cout << test.tm_sec << std::endl;
	std::cout << test.tm_year << std::endl;

	std::cout << ((TimeHelp::if_Precede("Sat Feb  4 17:10:08 2017", "Sat Feb  5 17:10:08 2017")) ? "Yes" : "No") << std::endl;
	system("pause");
}
#endif // DEBUG

 