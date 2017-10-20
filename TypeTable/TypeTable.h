#pragma once

/////////////////////////////////////////////////////////////////////////////
//  TypeTable.h - Containner for type analysis                             //
//  Language:     C++, VS 2015                                             //
//  Platform:     SurfaceBook, Windows 10 Pro                              //
//  Application:  Project1 for CSE687 - Object Oriented Design             //
//  Author:       Weijun Cai                                               //
/////////////////////////////////////////////////////////////////////////////
/*
*   Module Operations
*   -----------------
*   This module contains the delaration of TypeTable class, which
*   can store type info that TypeAnalysis gathers and display contents
*/
/*
*   Build Process
*   -------------
*   - Required files: TypeTable.h, TypeTable.cpp
*
*   Maintenance History
*   -------------------
*   ver 1.0 : 02 Mar 2017
*/


#include <unordered_map>
#include <set>
#include <utility>
class TypeTable
{
public:
	// aliaes for readability
	using FileSet = std::set<std::string>;
	using FileName = std::string;
	using TypeName = std::string;
	using FullTypeName = std::string;
	using TypePair = std::pair<FullTypeName, FileName>;
	using TypePairVec = std::vector<TypePair>;
	using TypeMap = std::unordered_map<TypeName, TypePairVec>;
	using Map = std::unordered_map<TypeName, FileName>;
	using FuncMap = std::unordered_map<std::string, FileSet>;

	// getter & setters for the underlying containers
	TypeMap& Type_Table();
	Map& Using_Name_Table();
	Map& Namespace_Table();
	Map& Typedef_Table();
	Map& Global_Var_Table();
	FuncMap& Func_Table();
	Map& H_CPP_Table();

	// display the contents of type table
	std::string show();
private:
	void show_type(std::ostream& out);
	void show_namespace(std::ostream& out);
	void show_alias(std::ostream& out);
	void show_typedef(std::ostream& out);
	void show_func(std::ostream& out);
	void show_global_var(std::ostream& out);

	TypeMap type_table;
	Map using_table;
	Map namespace_table;
	Map typedef_table;
	Map global_var_table;
	FuncMap func_table;

	Map h_cpp_dep_table;
};