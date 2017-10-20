#pragma once

/////////////////////////////////////////////////////////////////////////////
//  TimeHelp.h -  utility class for Query class                            //
//  Language:     C++, VS 2015                                             //
//  Platform:     SurfaceBook, Windows 10 Pro                              //
//  Application:  Project1 for CSE687 - Object Oriented Design             //
//  Author:       Weijun Cai                                               //
/////////////////////////////////////////////////////////////////////////////
/*
*   Module Operations
*   -----------------
*   This module contains the delaration of TimeHelp, which provides static helper 
*   functions for time interval query
*/
/*
*   Build Process
*   -------------
*   - Required files: TimeHelp.h, TimeHelp.cpp
*
*   Maintenance History
*   -------------------
*   ver 1.0 : 25 Jan 2017
*/

#include <string>
#include <ctime>
#include <map>

class TimeHelp
{
public:
	// determine if the first time is later than the second time
	bool if_Succeed(const std::string, const std::string);
	// determine if the first time is earlier than the second time
	bool if_Precede(const std::string, const std::string);
	// convert a time string into std::tm struct
	tm StringToTm(const std::string time);
private:
	 std::map<std::string, int> week = { { "Sun", 0 },{ "Mon", 1 },{ "Tue", 2 },{ "Wed", 3 },{ "Thu", 4 },{ "Fri", 5 },{ "Sat", 6 } };
	 std::map<std::string, int> month = { { "Jan", 0 },{ "Feb", 1 },{ "Mar", 2 },{ "Apr", 3 },{ "May", 4 },{ "Jun", 5 },{ "Jul", 6 },{ "Aug", 7 },{ "Sep", 8 },{ "Oct", 9 },{ "Nov", 10 },{ "Dec", 11 } };

};
