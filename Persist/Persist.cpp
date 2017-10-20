///////////////////////////////////////////////////////////////////////////////
//  Persist.cpp - convert/restore database contents into/from XML file/string//
//  Language:     C++, VS 2015                                               //
//  Platform:     SurfaceBook, Windows 10 Pro                                //
//  Application:  Project1 for CSE687 - Object Oriented Design               //
//  Author:       Weijun Cai                                                 //
///////////////////////////////////////////////////////////////////////////////


#include "Persist.h"


std::ifstream& operator >> (std::ifstream& ifs, std::string& s)
{
	char c;
	std::ostringstream oss;
	while (ifs.get(c))
		oss << c;
	s = oss.str();
	return ifs;
}


#ifdef PERSIST_CPP
void main()
{
	std::unordered_map<std::string, Element<std::string>> container;
	std::unordered_map<std::string, Element<std::string>> container2;
	Persistor<std::string> persistor;
	std::string xml;
	Element<std::string> elem1;
	elem1.name = "elem1";
	elem1.category = "test";
	elem1.description = "desc1";
	elem1.timeDate = "Jan 01 2017 12:10:8";
	elem1.data = "elem1's StrData";
	container["element1"] = elem1;
	Element<std::string> elem2;
	elem2.name = "elem2";
	elem2.category = "test";
	elem2.description = "desc2";
	elem2.timeDate = "Feb 04 2017 12:10:8";
	elem2.data = "elem2's StrData";
	elem2.children.getValue().insert("elem1");
	container["element2"] = elem2;
	Element <std::string> elem3;
	elem3.name = "elem3";
	elem3.category = "test";
	elem3.description = "desc3";
	elem3.timeDate = "Dec 22 2017 12:10:8";
	elem3.data = "elem3's StrData";
	elem3.children.getValue().insert("elem1");
	elem3.children.getValue().insert("elem2");
	container["element3"] = elem3;
	std::cout << "converting items to xml" << std::endl;
	xml = persistor.ToXml(container);
	std::cout << xml << std::endl;
	persistor.FromXml(container2, xml);
	std::cout << "restoring items from xml" << std::endl;
	for (auto item : container2)
	{
		std::cout << item.second.show();
	}
	persistor.SerializeToFile(container, "test.txt");
	persistor.DeserializeFromFile(container2, "test.txt");
	std::cout << "deserializing items from xml" << std::endl;
	for (auto item : container2)
	{
		std::cout << item.second.show();
	}
	persistor.SetAutoSaving(container2, 3, 0, "testsaving.txt");
	system("pause");
}
#endif // PERSIST_CPP

