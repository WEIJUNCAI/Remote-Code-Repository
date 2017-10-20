/////////////////////////////////////////////////////////////////////////////
//  Element.cpp   represents a data record in this NoSql database          //
//  Language:     C++, VS 2015                                             //
//  Platform:     SurfaceBook, Windows 10 Pro                              //
//  Application:  Project1 for CSE687 - Object Oriented Design             //
//  Author:       Weijun Cai                                               //
/////////////////////////////////////////////////////////////////////////////
/*
*   Module Operations
*   -----------------
*   This module contains the test stub of element class.
*/
/*
*   Build Process
*   -------------
*   - Required files: Element.h
*
*   Maintenance History
*   -------------------
*   ver 1.0 : 25 Jan 2017
*/

#include "Element.h"

#ifdef ELEMENT_CPP
void main()
{
	Element<std::string> strElem;
	strElem.name = "strElem";
	strElem.category = "cate1";
	strElem.description = "desc1";
	strElem.timeDate = "Mon Mar 15 16:01:57 2017";
	strElem.data = "string data";

	strElem.show();

	Element<int> intElem;
	intElem.name = "intElem";
	intElem.category = "cate2";
	intElem.description = "desc2";
	intElem.timeDate = "Mon Mar 15 16:01:57 2017";
	intElem.data = 8;

	intElem.show();
}

#endif // ELEMENT_CPP
