#pragma once

/////////////////////////////////////////////////////////////////////////////
//  Query.h -     Query contents of NoSQL database                         //
//  Language:     C++, VS 2015                                             //
//  Platform:     SurfaceBook, Windows 10 Pro                              //
//  Application:  Project1 for CSE687 - Object Oriented Design             //
//  Author:       Weijun Cai                                               //
/////////////////////////////////////////////////////////////////////////////
/*
*   Module Operations
*   -----------------
*   This module contains the delaration and implementation of Query class, 
*   which provides necessary facilities for database to query the data or metadata 
*   with certain patterns
*/
/*
*   Build Process
*   -------------
*   - Required files: Element.h, Convert.h, Convert.cpp, TimeHelp.h, TimeHelp.cpp
*                     Query.h
*   Maintenance History
*   -------------------
*   ver 1.0 : 25 Jan 2017
*/

#include <unordered_map>
#include <vector>
#include "../Convert/Convert.h"
#include "../Element/Element.h"
#include "../Query/TimeHelp.h"

template <typename Data>
class Query
{
	using Container = std::unordered_map<std::string, Element<Data>>;
	using Keys = std::set<std::string>;
public:
	// query the value of a specified key
	void value(Container& container, Keys& keys, std::vector<Element<Data>>& valuesToReturn);
	// query the children of a specified key
	void children(Container& container, Keys& keys, std::vector<std::set<std::string>>& childrenToReturn);
	// query keys with specified pattern
	void key(Container& container, Keys& keys, const std::string& pattern);
	// query keys whose name metadata fields contain specified string
	void name(Container& container, Keys& keys, const std::string& pattern);
	// query keys whose category metadata fields contain specified string
	void category(Container& container, Keys& keys, const std::string& pattern);
	// query keys whose data fields contain specified string
	void data(Container & container, Keys & keys, const std::string& pattern);
	// query keys whose values are updated within the specified time interval
	void time(Container& container, Keys& keys, const std::string& lowerbound, const std::string& upperbound);

private:
	TimeHelp timehelp;
};

template<typename Data>
void Query<Data>::value(Container& container, Keys& keys, std::vector<Element<Data>>& valuesToReturn)
{
	for (auto key : keys)
	{
		if (container.find(key) != container.end())
			valuesToReturn.push_back(container[key]);
		else
			valuesToReturn.push_back(Element<Data>());
	}
}

template<typename Data>
void Query<Data>::children(Container& container, Keys& keys, std::vector<std::set<std::string>>& childrenToReturn)
{
	for (auto key : keys)
	{
		if (container.find(key) != container.end())
			childrenToReturn.push_back(container[key].children.getValue());
		else
			childrenToReturn.push_back(std::vector<std::set<std::string>>());
	}
}

template<typename Data>
inline void Query<Data>::key(Container & container, Keys & keys, const std::string& pattern)
{
	for (auto item : container)
	{
		if (item.first.find(pattern) != std::string::npos)
			keys.insert(item.first);
		continue;
	}
}

template<typename Data>
void Query<Data>::name(Container & container, Keys& keys, const std::string& pattern)
{
	for (auto item : container)
	{
		if (item.second.name.getValue().find(pattern) != std::string::npos)
			keys.insert(item.first);
		continue;
	}
}

template<typename Data>
void Query<Data>::category(Container & container, Keys & keys, const std::string& pattern)
{
	for (auto item : container)
	{
		if (item.second.category.getValue().find(pattern) != std::string::npos)
			keys.insert(item.first);
		continue;
	}
}

template<typename Data>
void Query<Data>::data(Container & container, Keys & keys, const std::string& pattern)
{
	for (auto item : container)
	{
		if (Convert<Data>::toString(item.second.data.getValue()).find(pattern) != std::string::npos)
			keys.insert(item.first);
		continue;
	}
}

template<typename Data>
void Query<Data>::time(Container & container, Keys & keys, const std::string& lowerbound, const std::string& upperbound)
{
	for (auto item : container)
	{
		if (timehelp.if_Precede(item.second.timeDate.getValue(), upperbound) && timehelp.if_Succeed(item.second.timeDate.getValue(), lowerbound))
			keys.insert(item.first);
		continue;
	}
}
