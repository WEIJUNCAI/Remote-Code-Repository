#pragma once

/////////////////////////////////////////////////////////////////////////////
//  DependencyAnalyzer.h - Analyze dependencies among files                //
//  Language:     C++, VS 2015                                             //
//  Platform:     SurfaceBook, Windows 10 Pro                              //
//  Application:  Project1 for CSE687 - Object Oriented Design             //
//  Author:       Weijun Cai                                               //
/////////////////////////////////////////////////////////////////////////////
/*
*   Module Operations
*   -----------------
*   This module contains the delaration of DependencyAnalyzer class, which
*   can determine depdendency relationships given a set of file tokens and their type table,
*   analysis results are stored in a NoSQL database, and can save results
*   into XML file/string and content query.
*/
/*
*   Build Process
*   -------------
*   - Required files: TypeAnalysis.h, TypeAnalysis.cpp
*                     NoSqlDb.h
*                     DependencyAnalyzer.h, DependencyAnalyzer.cpp
*
*   Maintenance History
*   -------------------
*   ver 1.0 : 22 Feb 2017
*/

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "../TypeAnalysis/TypeAnalysis.h"
#include "../NoSqlDb/NoSqlDb.h"


class DependencyAnalyzer
{
public:
	DependencyAnalyzer();

	// biuld type table given the root AST node of processed files
	void AddFileRoot(const CodeAnalysis::ASTNode* rootNode);

    // process each statemnt based on its tokens
	void AnalyzeStatement(const std::string& file, Scanner::ITokCollection* Tokens);

	// add a new entry in DB for a file 
	void AddNewFile(const std::string& key, const std::string& category, const std::string& description);

	// return the DB containing all files and their dependency info
	NoSqlDb<std::string>& GetAnalyResult();

	// save analysis result into specified path
	void SaveAnalyResultToFile(const std::string& path);

	// display contents
	void DisplayTypeTable();
	void DisplayDependency();
private:
	//void TreeWalk_BF(const CodeAnalysis::ASTNode* node);
	/*void NodeAnalyze(const CodeAnalysis::ASTNode* node);*/
	std::unordered_set<std::string> skipTokens;
	TypeAnalysis table;
	NoSqlDb<std::string> dependencyDB;
};




