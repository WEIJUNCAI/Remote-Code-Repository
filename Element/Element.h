#pragma once

/////////////////////////////////////////////////////////////////////////////
//  Element.h -   represents a data record in this NoSql database          //
//  Language:     C++, VS 2015                                             //
//  Platform:     SurfaceBook, Windows 10 Pro                              //
//  Application:  Project1 for CSE687 - Object Oriented Design             //
//  Author:       Weijun Cai                                               //
/////////////////////////////////////////////////////////////////////////////
/*
*   Module Operations
*   -----------------
*   This module contains the definition and implementation of Element class
*   which is the value part of the key-value pair of database
*/
/*
*   Build Process
*   -------------
*   - Required files: CppProperties.h, Convert.h
*
*   Maintenance History
*   -------------------
*   ver 1.0 : 25 Jan 2017
*/


#include <string>
#include <set>
#include "../CppProperties/CppProperties.h"
#include "../Convert/Convert.h"


template<typename Data>
class Element
{
public:
	using Name = std::string;
	using Category = std::string;
	using TimeDate = std::string;
	using Description = std::string;
	using Children = std::set<std::string>;
	Element() = default;

	Property<Name> name;           // metadata
	Property<Category> category;    // metadata
	Property<TimeDate> timeDate;    // metadata
	Property<Description> description; //metadata
	Property<Children> children;  //metadata
	Property<Data> data;            // data

	// converting the content of element into string to dsiplay
	std::string show();
};

template<typename Data>
std::string Element<Data>::show()
{
	std::ostringstream out;
	out.setf(std::ios::adjustfield, std::ios::left);
	out << "\n    " << std::setw(8) << "name" << " : " << name;
	out << "\n    " << std::setw(8) << "category" << " : " << category;
	out << "\n    " << std::setw(8) << "timeDate" << " : " << timeDate;
	out << "\n    " << std::setw(8) << "description" << " : " << description;

	for (auto child : children.getValue())
	{
		out << "\n    " << std::setw(8) << "children" << " : " << child;
	}
	out << "\n    " << std::setw(8) << "data" << " : " << Convert<Data>::toString(data);
	out << "\n";
	return out.str();
}