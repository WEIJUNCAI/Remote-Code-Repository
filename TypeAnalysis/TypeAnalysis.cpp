/////////////////////////////////////////////////////////////////////////////
//  TypeAnalysis.cpp - Performing type analysis based on AST               //
//  Language:     C++, VS 2015                                             //
//  Platform:     SurfaceBook, Windows 10 Pro                              //
//  Application:  Project1 for CSE687 - Object Oriented Design             //
//  Author:       Weijun Cai                                               //
/////////////////////////////////////////////////////////////////////////////

#include <functional>
#include <iostream>
#include "TypeAnalysis.h"

using namespace CodeAnalysis;

void TypeAnalysis::BiuldTable(const ASTNode * globalNode)
{
	if (globalNode != nullptr)
	{
		TreeWalk_BF(globalNode);
		ProcessDecl(globalNode);
	}
}



std::set<std::string> TypeAnalysis::TypeLookup(const std::string& File, const std::string& Type)
{
	auto typePair = table.Type_Table().find(Type);
	auto funcPair = table.Func_Table().find(Type);
	std::set<std::string> result;
	// match in type table
	if (typePair != table.Type_Table().end())
	{
		// for all types with the same short name but different fully-qualified names
		for (auto fullname_file_pair : typePair->second)
			// type not defined in this file
			if (fullname_file_pair.second != File)
				result.insert(fullname_file_pair.second);
	}
	// no match in func table
	else if (funcPair != table.Func_Table().end())
	{
		for (auto file : funcPair->second)
			result.insert(file);
		// type defined int this file, remove this file from dependency
		if (funcPair->second.find(File) != funcPair->second.end())
			result.erase(File);
	}
	return std::move(result);
}

void TypeAnalysis::Display()
{
	std::cout << table.show() << std::endl;
}

void TypeAnalysis::ProcessNode(const ASTNode * node, const std::string& scope)
{
	auto path = node->path_;
	auto name = node->name_;
	//std::cout << "File: " << path << " Type: " << name << " Full name: " << scope << std::endl;
	if (node->type_ == "function") // if is a global function
		table.Func_Table()[name].insert(path);
	else                           // a class, struct or enum
	{
		table.Type_Table()[name].push_back(TypeTable::TypePair(scope, path));
	}
}

void TypeAnalysis::ProcessDecl(const CodeAnalysis::ASTNode * node)
{
	for (auto decl : node->decl_)
	{
		auto & filename = decl.package_;
		auto & tokenCollc = *decl.pTc;
		if (decl.declType_ == usingDecl) // process using statement 
		{
			if (tokenCollc[1] == "namespace")
			{
				if (tokenCollc[2] == "::")
					table.Namespace_Table()[tokenCollc[3]] = filename;
				//std::cout << "File: " << filename << " using alias: " << tokenCollc[1] << std::endl;
				else
					table.Namespace_Table()[tokenCollc[2]] = filename;
			}
			else
			{
				table.Using_Name_Table()[tokenCollc[1]] = filename;
				//std::cout << "File: " << filename << " namespace import: " << tokenCollc[2] << std::endl;
			}
		}
		else if (tokenCollc.find("typedef") < tokenCollc.length()) // process typedefs
		{
			table.Typedef_Table()[tokenCollc[tokenCollc.find("typedef") + 1]] = filename;
		}
		else if (tokenCollc.find("=") < tokenCollc.length()) // process global data
		{
			table.Global_Var_Table()[tokenCollc[tokenCollc.find("=") - 1]] = filename;
		}
	}
}

std::unordered_map<std::string, std::string>& TypeAnalysis::Get_H_CPP_dep()
{
	return table.H_CPP_Table();
}


void TypeAnalysis::TreeWalk_BF(const CodeAnalysis::ASTNode * node)
{
	Walk_Queue walkQ(node);
	while (walkQ.NodeQ().size() != 0)
	{
		auto currentNode = walkQ.NodeQ().front();  // get current node
		std::string currentScope;
		std::string currentPackage;
		bool isMemFunc;
		walkQ.NodeQ().pop();
		std::size_t realChildNum = 0; // number of child node with specified type
		currentScope = walkQ.ScopeQ().front() + "::" + currentNode->name_;

		WalkHelper_AddChildren(currentNode, walkQ.NodeQ(), realChildNum);

		isMemFunc = WalkHelper_ProcessMemFuncs(currentNode, walkQ);

		if (currentNode->type_ != "namespace" && !isMemFunc)
			ProcessNode(currentNode, std::string(currentScope).erase(0, 2));

		WalkHelper_LeavingScope(currentNode, walkQ, currentScope, realChildNum);
	}
}

void TypeAnalysis::WalkHelper_AddChildren(const CodeAnalysis::ASTNode * currentNode, std::queue<const CodeAnalysis::ASTNode*>& walkQueue, size_t & realChildNum)
{
	for (auto childNode : currentNode->children_)  // add children to queue
	{
		if (filterSet.find(childNode->type_) != filterSet.end()) // only specified types of nodes will be analyzed
		{
			walkQueue.push(childNode);
			++realChildNum;
		}
	}
}

bool TypeAnalysis::WalkHelper_ProcessMemFuncs(const CodeAnalysis::ASTNode * currentNode, Walk_Queue& walkQ)
{
	if (currentNode->type_ == "function" && walkQ.TypeQ().front() != "namespace") // member function
	{
		std::string packageName = currentNode->path_;
		std::string parentType = walkQ.TypeQ().front();
		size_t index = packageName.find(".cpp");
		if (index != std::string::npos && (parentType == "class" || parentType == "struct"))
			table.H_CPP_Table()[walkQ.PackageQ().front()] = currentNode->path_;
		return true;
	}
	return false;
}

void TypeAnalysis::WalkHelper_LeavingScope(const CodeAnalysis::ASTNode* currentNode, Walk_Queue& walkQ, std::string& currentScope, size_t& realChildNum)
{
	walkQ.ChildrenCountQ().front()--;  // leaving current scope, decrement counter
	if (walkQ.ChildrenCountQ().front() < 1) // leaving current scope, check if this node is the last child of its parent scope
	{
		walkQ.ChildrenCountQ().pop();
		walkQ.ScopeQ().pop();
		walkQ.TypeQ().pop();
		walkQ.PackageQ().pop();
	}
	if (realChildNum != 0) // 
	{
		walkQ.PackageQ().push(currentNode->path_); // saving parent package path (for header-cpp dependency analysis based on member funcs)
		walkQ.TypeQ().push(currentNode->type_); // saving parent type (namespace or class or struct, to distiguish between member and globla funcs)
		walkQ.ChildrenCountQ().push(realChildNum);  // add chidren number to queue (exclude control...etc)
		walkQ.ScopeQ().push(currentScope); // update scope info
	}  // if current node has no child, its scope need not to be saved
}

#ifdef DEBUG_TYPE



void main()
{
	ScopeStack<ASTNode*> stack;
	AbstrSynTree ast(stack);
	ASTNode* pX = new ASTNode("class", "X");
	pX->package_ = "P1";
	ast.add(pX);                                        // add X scope
	ASTNode* pf1 = new ASTNode("function", "f1");
	ast.add(pf1);                                       // add f1 scope
	ASTNode* pc1 = new ASTNode("control", "if");
	ast.add(pc1);                                       // add c1 scope
	ast.pop();                                          // end c1 scope
	ast.pop();                                          // end f1 scope
	ASTNode* pf2 = new ASTNode("function", "f2");
	ast.add(pf2);                                       // add f2 scope
	ast.pop();                                          // end f2 scope
	ast.pop();                                          // end X scope
	ASTNode* nss = new ASTNode("namespace", "namesp2");
	ast.add(nss);
	ASTNode* py = new ASTNode("class", "Y");
	py->package_ = "P1";
	ast.add(py);                                        // add X scope
	ASTNode* pyf1 = new ASTNode("function", "f1");
	ast.add(pyf1);                                       // add f1 scope
	ASTNode* pyc1 = new ASTNode("control", "if");
	ast.add(pyc1);                                       // add c1 scope
	ast.pop();                                          // end c1 scope
	ast.pop();                                          // end f1 scope
	ASTNode* pyf2 = new ASTNode("function", "f2");
	ast.add(pyf2);                                       // add f2 scope
	ast.pop();                                          // end f2 scope
	ast.pop();                                         // end X scope
	ast.pop();
	auto rootNode = ast.root();

	TypeTable tb;

	tb.add(rootNode);
	auto result = tb.TypeLookup("P1", "X");
	for (auto file : result)
	{
		std::cout << file << std::endl;
	}
	system("pause");
}
#endif // DEBUG_TYPE