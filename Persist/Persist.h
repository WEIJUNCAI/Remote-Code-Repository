#pragma once

/////////////////////////////////////////////////////////////////////////////
//  Persist.h - convert/restore database contents into/from XML file/string//
//  Language:     C++, VS 2015                                             //
//  Platform:     SurfaceBook, Windows 10 Pro                              //
//  Application:  Project1 for CSE687 - Object Oriented Design             //
//  Author:       Weijun Cai                                               //
/////////////////////////////////////////////////////////////////////////////
/*
*   Module Operations
*   -----------------
*   This module contains the delaration and implementation of Persistor class,
*   which is used to store database contents into XML string/file, and restore or 
*   augment database contents from XML string/file.
*/
/*
*   Build Process
*   -------------
*   - Required files: Element.h, Convert.h, StrHelper.h, XmlDocument.h, XmlElement.h
*                     Persist.h, Persist.cpp
*   Maintenance History
*   -------------------
*   ver 1.0 : 25 Jan 2017
*/

#include <unordered_map>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <set>
#include <iomanip>
#include <iostream>
#include <ctime>
#include <thread>
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"
#include "../Element/Element.h"
#include "../Convert/Convert.h"
#include "StrHelper.h"



template <typename Data>
class Persistor
{
	using SPtr = std::shared_ptr<XmlProcessing::AbstractXmlElement>;
	using Container = std::unordered_map<std::string, Element<Data>>;
public:
	Persistor() = default;

	// store DB contents into XML file located in specified path  
	bool SerializeToFile(Container& container, const std::string& path);
	// restore DB contents from XML file located in specified path
	bool DeserializeFromFile(Container& container, const std::string& path);
	// turn on/off auto saving 
	void SetAutoSaving(Container& container,  unsigned int timeInterval,  unsigned int numberOfWrites, const std::string& s);
	// if turned on, automatically persist DB contents after certain number of writes
	void WriteSaving(Container& container, const std::string& path);
    // convert DB contents into XML string 
	std::string ToXml(Container& container);
	// restore DB contents from XML string
	void FromXml(Container& container, const std::string& xml);
private:
	// helper function, creating child nodes with specified tag and text.
	void CreateChildNode(SPtr parent, const std::string& tag, const std::string& text);
	// the child thread used to automatically save DB contents after a certain period of time
	std::thread timeSavingThread; 
	// the number of writes after which DB should be persisted 
	unsigned int writeCount = 0;
	// current write count
	unsigned int currentCount = 0;

};

template <typename Data>
void Persistor<Data>::CreateChildNode(SPtr parent, const std::string& tag, const std::string& text)
{
	SPtr Tag = XmlProcessing::makeTaggedElement(tag);
	parent->addChild(Tag);
	SPtr value = XmlProcessing::makeTextElement(text);
	Tag->addChild(value);
}


template <typename Data>
std::string Persistor<Data>::ToXml(Container& container)
{
	std::string xml;
	XmlProcessing::XmlDocument doc;
	SPtr root = XmlProcessing::makeTaggedElement("NoSQL");
	doc.docElement() = root;

	for (std::pair<std::string, Element<Data>> entry : container)
	{
		SPtr item = XmlProcessing::makeTaggedElement(entry.first);
		root->addChild(item);

		SPtr metaData = XmlProcessing::makeTaggedElement("metadata");
		item->addChild(metaData);
		CreateChildNode(metaData, "name", entry.second.name);
		CreateChildNode(metaData, "category", entry.second.category);
		CreateChildNode(metaData, "time date", entry.second.timeDate);
		CreateChildNode(metaData, "description", entry.second.description);
		SPtr childrenSet = XmlProcessing::makeTaggedElement("children");
		metaData->addChild(childrenSet);
		for (auto childKey : entry.second.children.getValue())
		{
			CreateChildNode(childrenSet, childKey, childKey);
		}

		SPtr dataTag = XmlProcessing::makeTaggedElement("data");
		item->addChild(dataTag);
		SPtr data = XmlProcessing::makeTextElement(Convert<Data>::toString(entry.second.data));
		dataTag->addChild(data);
	}

	xml = doc.toString();
	return xml;
}

template<typename Data>
void Persistor<Data>::FromXml(Container& container, const std::string& xml)
{
	XmlProcessing::XmlDocument doc(xml, XmlProcessing::XmlDocument::str);
	std::vector<SPtr> v = doc.xmlRoot()->children();

	for (auto item : v)
	{
		Element<Data> entry = Element<Data>();
		//                           metadata    name          text          value
		entry.name = trim(item->children()[0]->children()[0]->children()[0]->value());
		entry.category = trim(item->children()[0]->children()[1]->children()[0]->value());
		entry.timeDate = trim(item->children()[0]->children()[2]->children()[0]->value());
		entry.description = trim(item->children()[0]->children()[3]->children()[0]->value());
		for (auto child : item->children()[0]->children()[4]->children())
		{
			entry.children.getValue().insert(trim(child->children()[0]->value()));
		}
		//                                                 data     text           value
		entry.data = Convert<Data>::fromString(item->children()[1]->children()[0]->value());

		container[item->tag()] = entry;
	}
}

template<typename Data>
inline bool Persistor<Data>::SerializeToFile(Container & container, const std::string& path)
{
	std::ofstream ofs(path);
	if (ofs.good())
	{
		std::string xml = ToXml(container);
		ofs << xml;
		return true;
	}
	else
		return false;
}

template<typename Data>
inline bool Persistor<Data>::DeserializeFromFile(Container & container, const std::string& path)
{
	std::ifstream ifs(path);
	if (ifs.good())
	{
		std::string xml;
		ifs >> xml;
		FromXml(container, xml);
		return true;
	}
	else
		return false;
}

std::ifstream& operator >> (std::ifstream& ifs, std::string& s);

template<typename Data>
void Persistor<Data>::SetAutoSaving(Container& container, unsigned int timeInterval, const unsigned int numberOfWrites, const std::string& path)
{
	if (timeInterval > 0)
		timeSavingThread = std::thread([&, timeInterval, path] 
	{while (true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(timeInterval));
		std::cout << std::endl << "Auto saving set to every " << timeInterval << " seconds, now saving..." << std::endl;
		SerializeToFile(container, path);
	}});
	if (numberOfWrites > 0)
		writeCount = numberOfWrites;
}


template<typename Data>
void Persistor<Data>::WriteSaving(Container& container, const std::string& path)
{
	++currentCount;
	if (writeCount == 0)
		return;
	else if(currentCount >= writeCount)
	{
		currentCount = 0;
		SerializeToFile(container, path);
		std::cout << "Auto saving set to every " << writeCount << " writes, now saving..." << std::endl;
	}	
	return;	
}


template<typename Data>
void TimedSaving(std::unordered_map<std::string, Element<Data>>& container, Persistor<Data>& persistor, unsigned int interval)
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(interval));
		std::cout << "Auto saving set to every " << interval << " seconds, now saving..." << std::endl;
		persistor.SerializeToFile(container, "ttt.txt");
	}
}