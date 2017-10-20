/////////////////////////////////////////////////////////////////////////////
//  Query.cpp  -  Query contents of NoSQL database                         //
//  Language:     C++, VS 2015                                             //
//  Platform:     SurfaceBook, Windows 10 Pro                              //
//  Application:  Project1 for CSE687 - Object Oriented Design             //
//  Author:       Weijun Cai                                               //
/////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include "Query.h"

#ifdef QUERY_CPP
void main()
{
	std::unordered_map<std::string, Element<std::string>> container;
	std::set<std::string> queryResult;
	Query<std::string> query;
	std::string pattern("el");
	std::string pattern2("t");
	Element<std::string> elem1;
	elem1.name = "elem1";
	elem1.category = "test";
	elem1.description = "desc1";
	elem1.timeDate = "Jan 01 2017 12:10:8";
	elem1.data = "elem1's StrData";
	container["element1"] = elem1;
	Element<std::string> elem2;
	elem1.name = "elem2";
	elem1.category = "test";
	elem1.description = "desc2";
	elem1.timeDate = "Feb 04 2017 12:10:8";
	elem1.data = "elem2's StrData";
	container["element2"] = elem2;
	Element <std::string> elem3;
	elem1.name = "elem3";
	elem1.category = "test";
	elem1.description = "desc3";
	elem1.timeDate = "Dec 22 2017 12:10:8";
	elem1.data = "elem3's StrData";
	container["element3"] = elem3;
	query.key(container, queryResult, pattern);
	for (auto key : queryResult)
		std::cout << key << std::endl;
	queryResult.clear();
	query.name(container, queryResult, pattern);
	for (auto key : queryResult)
		std::cout << key << std::endl;
	queryResult.clear();
	query.category(container, queryResult, pattern2);
	for (auto key : queryResult)
		std::cout << key << std::endl;
	queryResult.clear();

	query.data(container, queryResult, pattern);
	for (auto key : queryResult)
		std::cout << key << std::endl;
	queryResult.clear();

	system("pause");
}
#endif // QUERY_CPP

