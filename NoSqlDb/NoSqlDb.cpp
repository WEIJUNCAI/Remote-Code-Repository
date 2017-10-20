/////////////////////////////////////////////////////////////////////////////
//  NoSqlDb.cpp - key/value pair in-memory database                        //
//  Language:     C++, VS 2015                                             //
//  Platform:     SurfaceBook, Windows 10 Pro                              //
//  Application:  Project1 for CSE687 - Object Oriented Design             //
//  Author:       Weijun Cai                                               //
/////////////////////////////////////////////////////////////////////////////


#include "NoSqlDb.h"
#include <iostream>


using StrData = std::string;
using intData = int;
using Key = NoSqlDb<StrData>::Key;
using Keys = NoSqlDb<StrData>::Keys;

#ifdef NOSQL

int main()
{
	std::cout << "\n  Creating and saving NoSqlDb elements with string data";
	std::cout << "\n -------------------------------------------------------\n";
	NoSqlDb<StrData> db, db2, db3;
	Element<StrData> elem1;
	elem1.name = "elem1";
	elem1.category = "test";
	elem1.description = "desc1";
	elem1.timeDate = "Jan 01 2017 12:10:8";
	elem1.data = "elem1's StrData";
	db.Add_entry(elem1.name, elem1); //name is the key?
	Element<StrData> elem2;
	elem2.name = "elem2";
	elem2.category = "test";
	elem2.description = "desc2";
	elem2.timeDate = "Jan 02 2017 10:10:6";
	elem2.data = "elem2's StrData";
	db.Add_entry(elem2.name, elem2);
	auto xml = db.ToXml();
	std::cout << xml << std::endl;
	db2.FromXml(xml);
	std::cout << "\n  Retrieving elements from NoSqlDb<string>";
	std::cout << "\n ------------------------------------------\n";
	Keys keys = db2.keys();
	for (Key key : keys)
	{
		std::cout << "\n  " << key << ":";
		std::cout << db.value(key).show();
	}
	std::cout << "\n  Editing elements from NoSqlDb<string>";
	std::cout << "\n ------------------------------------------\n";
	db2.Edit_name("elem2", "elem22");
	db2.Edit_category("elem2", "category22");
	db2.Edit_description("elem2", "desc22");
	db2.Edit_data("elem2", "string value");
	db2.Add_child("elem2", "elem1");
	std::cout << "\n  Edited db2:" << std::endl;
	std::cout << db2.ToXml() << std::endl;
}


#endif // NOSQL
