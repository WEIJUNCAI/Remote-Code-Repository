#pragma once

/////////////////////////////////////////////////////////////////////////////
//  TypeAnalysis.h - Performing type analysis based on AST                 //
//  Language:     C++, VS 2015                                             //
//  Platform:     SurfaceBook, Windows 10 Pro                              //
//  Application:  Project1 for CSE687 - Object Oriented Design             //
//  Author:       Weijun Cai                                               //
/////////////////////////////////////////////////////////////////////////////
/*
*   Module Operations
*   -----------------
*   This module contains the delaration of TypeAnalysis class, which
*   can extract type info from a given AST, gather all types, using,
*   typedef, global function and data.
*/
/*
*   Build Process
*   -------------
*   - Required files: TypeTable.h, TypeTable.cpp
*                     AbstrSynTree.h, AbstrSynTree.cpp
*                     TypeAnalysis.h, TypeAnalysis.cpp
*   Maintenance History
*   -------------------
*   ver 1.0 : 22 Feb 2017
*/

#include <set>
#include <stack>
#include <vector>
#include <memory>
#include <unordered_set>
#include "../AbstractSyntaxTree/AbstrSynTree.h"
#include "../TypeTable/TypeTable.h"

class Walk_Queue;

class TypeAnalysis
{
public:
	// using the specified AST to construct type table, AST should be the result of parsing all files
	void BiuldTable(const CodeAnalysis::ASTNode* globalNode);

	// given a type name and file in which it appears, return the file it is defined
	std::set<std::string> TypeLookup(const std::string& File, const std::string& Type);

	// used for header file and implementation file denpendency analysis
	std::unordered_map<std::string, std::string>& Get_H_CPP_dep();

	// display the contents of type table
	void Display();

private:

	// traverse AST and resolve namespaces
	void TreeWalk_BF(const CodeAnalysis::ASTNode* node);

	void WalkHelper_AddChildren(const CodeAnalysis::ASTNode* currentNode, std::queue<const CodeAnalysis::ASTNode*>& walkQueue, size_t& realChildNum);

	bool WalkHelper_ProcessMemFuncs(const CodeAnalysis::ASTNode* currentNode, Walk_Queue& walkQ);

	void WalkHelper_LeavingScope(const CodeAnalysis::ASTNode* currentNode, Walk_Queue& walkQ, std::string& currentScope, size_t& realChildNum);
	// called by treewalk on every node needs to be processed, add to corresponding table
	void ProcessNode(const CodeAnalysis::ASTNode* node, const std::string& scope);

	// process global statements for using, typedef, global data
	void ProcessDecl(const CodeAnalysis::ASTNode* node);
private:
	TypeTable table;

	std::unordered_set<std::string> filterSet{ "namespace", "class", "struct", "enum", "function" };
};


class Walk_Queue
{
public:
	Walk_Queue(const CodeAnalysis::ASTNode * rootNode)
	{
		walkQueue.push(rootNode);
		scopeQueue.push("");
		childrenCountQueue.push(1);
		typeQueue.push("namespace");
		packageQueue.push("");
	}

	std::queue<const CodeAnalysis::ASTNode*>& NodeQ() { return walkQueue; }
	std::queue<std::string>& ScopeQ() { return scopeQueue; }
	std::queue<int>& ChildrenCountQ() { return childrenCountQueue; }
	std::queue<std::string>& TypeQ() { return typeQueue; }
	std::queue<std::string>& PackageQ() { return packageQueue; }

private:
	std::queue<const CodeAnalysis::ASTNode*> walkQueue;
	std::queue<std::string> scopeQueue;
	std::queue<int> childrenCountQueue;
	std::queue<std::string> typeQueue;
	std::queue<std::string> packageQueue;
};