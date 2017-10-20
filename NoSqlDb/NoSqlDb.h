#pragma once

/////////////////////////////////////////////////////////////////////////////
//  NoSqlDb.h -   key/value pair in-memory database                        //
//  Language:     C++, VS 2015                                             //
//  Platform:     SurfaceBook, Windows 10 Pro                              //
//  Application:  Project1 for CSE687 - Object Oriented Design             //
//  Author:       Jim Fawcett, Weijun Cai                                  //
/////////////////////////////////////////////////////////////////////////////
/*
*   Module Operations
*   -----------------
*   This module contains the delaration and implementation of NoSqlDb class, which 
*   stores custom data types and matadata as value of keys, supporting serialization/deserialization
*   into/from XML file/string and content query.
*/
/*
*   Build Process
*   -------------
*   - Required files: Element.h
*                     Persist.h
*                     Query.h
*                     NoSqlDb.h
*
*   Maintenance History
*   -------------------
*   ver 1.1 : 2 Mar 2017
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
#include <utility>
#include <algorithm>
#include <iterator>
#include "../Element/Element.h"
#include "../Persist/Persist.h"
#include "../Query/Query.h"


/////////////////////////////////////////////////////////////////////
// NoSqlDb class is a key/value pair in-memory database
// - stores and retrieves elements

template<typename Data>
class NoSqlDb
{
public:
	using Key = std::string;
	using Keys = std::vector<Key>;
	
	using iterator = typename std::unordered_map<Key, Element<Data>>::iterator;

	bool find(const Key&);

	iterator begin();
	iterator cbegin();
	iterator end();
	iterator cend();

	Element<Data> operator[](const Key& index) const;
	Element<Data>& operator[](const Key& index);

	// edit metadata or data field of each item
	bool Add_entry(const Key& key, Element<Data> elem);  //if not exist in DB, add to collection
	bool Add_entry(const Key& key, const std::string& categoty, const std::string& decription, const Data& data);  //if not exist in DB, add to collection 
	bool Del_entry(const Key& key);
	bool Edit_name(const Key& key, const std::string& s);
	bool Edit_category(const Key& key, const std::string& s);
	bool Edit_description(const Key& key, const std::string& s);
	bool Add_child(const Key& key, const Key& child);
	bool Del_child(const Key& key, const Key& child);
	bool Edit_data(const Key& key, Data data);
	bool Add_To_Children(const Key& key, std::set<std::string>& setToAdd);


	void SetAutoSaving(unsigned int write, unsigned int time, const std::string path) { savePath = path; persistor.SetAutoSaving(store, time, write, savePath); }
	// converting/retrieving contents of database into/from xml string
	std::string ToXml() { return persistor.ToXml(store); }
	void FromXml(std::string xml) { persistor.FromXml(store, xml); }

	// persist/restore db into/from file
	bool Serialize(const std::string& path) { return persistor.SerializeToFile(store, path); }
	bool Deserialize(const std::string& path) { return persistor.DeserializeFromFile(store, path); }

	// query the value/children of a list of keys
	std::vector<Element<Data>> value() { query.value(store, found, found_value); return std::move(found_value); }
	std::vector<std::set<Key>> children() { query.children(store, found, found_children); return std::move(found_children);}

	// query the value/children of a specified key
	Element<Data> value(const Key& key); 
	std::set<Key> children(const Key& key);

	// query keys with metadata/data containing specified patterns
	NoSqlDb<Data>& Query_key(const std::string& pattern) { query.key(store, found, pattern); return *this; }
	NoSqlDb<Data>& Query_name(const std::string& pattern) { query.name(store, found, pattern); return *this; }
	NoSqlDb<Data>& Query_category(const std::string& pattern) { query.category(store, found, pattern); return *this; }
	NoSqlDb<Data>& Query_data(const std::string& pattern) { query.data(store, found, pattern); return *this; }
	NoSqlDb<Data>& Query_time(const std::string& lowerbound, std::string upperbound) { query.time(store, found, lowerbound, upperbound); return *this; }
	NoSqlDb<Data>& Query_time(const std::string& bound);

	// return last query result
	std::set<std::string> select() { return std::move(found); }

	// return the union of previous query results
	std::set<std::string> Get_union(std::set<std::string>& queryResult1, std::set<std::string>& queryResult2);
	
	// helper function, get the string form of current time
	std::string GetCurrentTime();

	// get all keys in DB
	Keys keys();

	//number of entries
	size_t size();  

	// dispaly the contents of DB on screen
	void show();
	

private:
	// key-value pair
	using Item = std::pair<Key, Element<Data>>; 
	// container storing all entries
	std::unordered_map<Key, Element<Data>> store; 

	// an instance of Persistor class, used to persist data into/restore data from XML files
	Persistor<Data> persistor; 
	// default file saving path
	std::string savePath;

	//an instance of Query class, used to access query methods within 
	Query<Data> query;
	// last query result 
	std::set<Key> found = {};
	// last result of value query
	std::vector<Element<Data>> found_value;
	// last result of children query
	std::vector<std::set<Key>> found_children;

};

template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::keys()
{
	Keys keys;
	for (auto item : store)
	{
		keys.push_back(item.first);
	}
	return keys;
}





template<typename Data>
inline bool NoSqlDb<Data>::find(const Key & key)
{
	if (store.find(key) != store.end())
	{
		return true;
	}
	return false;
}

template<typename Data>
inline typename NoSqlDb<Data>::iterator NoSqlDb<Data>::begin()
{
	return store.begin();
}


template<typename Data>
inline typename NoSqlDb<Data>::iterator NoSqlDb<Data>::cbegin()
{
	return store.begin();
}

template<typename Data>
inline typename NoSqlDb<Data>::iterator NoSqlDb<Data>::end()
{
	return store.end();
}

template<typename Data>
inline typename NoSqlDb<Data>::iterator NoSqlDb<Data>::cend()
{
	return cend();
}

template<typename Data>
inline Element<Data> NoSqlDb<Data>::operator[](const Key& index) const
{
	return store[index];
}

template<typename Data>
inline Element<Data>& NoSqlDb<Data>::operator[](const Key& index)
{
	return store[index];
}

template<typename Data>
bool NoSqlDb<Data>::Add_entry(const Key& key, Element<Data> elem)
{
	if (store.find(key) != store.end())
		return false;

	//add time stamp

	std::string s = GetCurrentTime();
	s.erase(24, 2);

	elem.timeDate = s;
	store[key] = elem;
	std::cout << "Adding entry \"" << key << "\" to database..." << std::endl;
	persistor.WriteSaving(store, savePath);
	return true;
}

template<typename Data>
inline bool NoSqlDb<Data>::Add_entry(const Key & key, const std::string & categoty, const std::string & decription, const Data & data)
{
	if (store.find(key) == store.end())
	{
		Element<Data> &elem = store[key];
		elem.timeDate = GetCurrentTime();
		elem.name = key;
		elem.category = categoty;
		elem.description = decription;
		elem.data = data;
		return true;
	}
	return false;
}

template<typename Data>
bool NoSqlDb<Data>::Del_entry(const Key& key)
{
	if (store.find(key) == store.end())
		return false;
	//delete it from the children collection from all of its parents
	std::cout << "Deleting entry \"" << key << "\" from database..." << std::endl;
	store.erase(key);
	for (auto &item : store)
	{
		item.second.children.getValue().erase(key);
	}
	return true;
}

template<typename Data>
bool NoSqlDb<Data>::Edit_name(const Key& key, const std::string& s)
{
	if (store.find(key) != store.end())
	{
		std::cout << "Editing the name field of key \"" << key << "\"..." << std::endl;
		store[key].name = s;
		// update last edit time
		std::string s = GetCurrentTime();
		store[key].timeDate = s.erase(24, 2);
		persistor.WriteSaving(store, savePath);
		return true;
	}
	else
		return false;
}

template<typename Data>
bool NoSqlDb<Data>::Edit_category(const Key& key, const std::string& s)
{
	if (store.find(key) != store.end())
	{
		std::cout << "Editing the category field of key \"" << key << "\"..." << std::endl;
		store[key].category = s;
		// update last edit time
		std::string s = GetCurrentTime();
		store[key].timeDate = s.erase(24, 2);
		persistor.WriteSaving(store, savePath);
		return true;
	}
	else
		return false;
}

template<typename Data>
bool NoSqlDb<Data>::Edit_description(const Key& key, const std::string& s)
{
	if (store.find(key) != store.end())
	{
		std::cout << "Editing the description field of key \"" << key << "\"..." << std::endl;
		store[key].description = s;
		// update last edit time
		std::string s = GetCurrentTime();
		store[key].timeDate = s.erase(24, 2);
		persistor.WriteSaving(store, savePath);
		return true;
	}
	else
		return false;
}

template<typename Data>
bool NoSqlDb<Data>::Add_child(const Key& key, const Key& child)
{
	if (store.find(key) != store.end())
	{
		std::cout << "Adding child " << child << " to key "  << key << std::endl;
		auto &i = store[key].children.getRef();
		i.insert(child);
		/*store[key].children.getValue().insert(child);*/
		// update last edit time
		std::string s = GetCurrentTime();
		store[key].timeDate = s.erase(24, 2);
		persistor.WriteSaving(store, savePath);
		return true;
	}
	else
		return false;
}

template<typename Data>
bool NoSqlDb<Data>::Del_child(const Key& key, const Key& child)
{
	if (store.find(key) != store.end())
	{
		std::cout << "Deleting child " << child << " from key " << key << std::endl;

		store[key].children.getRef().erase(child);
		// update last edit time
		std::string s = GetCurrentTime();
		store[key].timeDate = s.erase(24, 2);
		persistor.WriteSaving(store, savePath);
		return true;
	}
	else
		return false;
}

template<typename Data>
inline bool NoSqlDb<Data>::Add_To_Children(const Key & key, std::set<std::string>& setToAdd)
{
	for (auto childToAdd : setToAdd)
		store[key].children.getRef().insert(childToAdd);

	return true;
}

template<typename Data>
inline bool NoSqlDb<Data>::Edit_data(const Key& key, Data data)
{
	if (store.find(key) != store.end())
	{
		std::cout << "Editing the data field of key \"" << key << "\"..." << std::endl;
		store[key].data = data;
		// update last edit time
		std::string s = GetCurrentTime();
		store[key].timeDate = s.erase(24, 2);
		persistor.WriteSaving(store, savePath);
		return true;
	}
	else
		return false;
}

template<typename Data>
NoSqlDb<Data>& NoSqlDb<Data>::Query_time(const std::string& bound)
{
	std::time_t t = std::time(0);
	char st[26];
	ctime_s(st, 26, &t);
	std::string s = st;

	query.time(store, found, bound, s);
	return *this;
}

template<typename Data>
std::set<std::string> NoSqlDb<Data>::Get_union(std::set<std::string>& queryResult1, std::set<std::string>& queryResult2)
{
	std::set<std::string> result;
	std::set_union(queryResult1.begin(), queryResult1.end(), queryResult2.begin(), queryResult2.end(), std::inserter(result, result.begin()));
	return result;
}

template<typename Data>
Element<Data> NoSqlDb<Data>::value(const Key& key)
{
	if (store.find(key) != store.end())
		return store[key];
	return Element<Data>();
}

template<typename Data>
std::set<std::string> NoSqlDb<Data>::children(const Key& key)
{
	if (store.find(key) != store.end())
		return store[key].children.getValue();
	return std::set<Key>();
}

template<typename Data>
size_t NoSqlDb<Data>::size()
{
	return store.size();
}

template<typename Data>
void NoSqlDb<Data>::show()
{
	for (auto item : store)
	{
		std::cout << item.second.show() << std::endl;
	}
}

template<typename Data>
inline std::string NoSqlDb<Data>::GetCurrentTime()
{
	std::time_t t = std::time(0);
	char st[26];
	ctime_s(st, 26, &t);
	std::string s = st;
	return s.erase(24, 2);
}