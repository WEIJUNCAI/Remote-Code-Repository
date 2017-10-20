/////////////////////////////////////////////////////////////////////////////
//  DependencyAnalyzer.cpp - Analyze dependencies among files              //
//  Language:     C++, VS 2015                                             //
//  Platform:     SurfaceBook, Windows 10 Pro                              //
//  Application:  Project1 for CSE687 - Object Oriented Design             //
//  Author:       Weijun Cai                                               //
/////////////////////////////////////////////////////////////////////////////


#include <functional>
#include "DependencyAnalyzer.h"

using namespace CodeAnalysis;

DependencyAnalyzer::DependencyAnalyzer() :
	skipTokens({ "if", "for", "while", "return", "using", "const", "auto", "void", "std", "main" })
{}

void DependencyAnalyzer::AddFileRoot(const CodeAnalysis::ASTNode * rootNode)
{
	if (rootNode != nullptr && rootNode->type_ == "namespace" && rootNode->name_ == "Global Namespace")
	{
		table.BiuldTable(rootNode);
	}
}

// process each statemnt based on tokens
void DependencyAnalyzer::AnalyzeStatement(const std::string& file, Scanner::ITokCollection* Tokens)
{
	// process each token
	for (auto token : *Tokens)
	{
		if (isalpha(token[0]) && skipTokens.find(token) == skipTokens.end())
		{
			// match in file table
			auto resultSet = table.TypeLookup(file, token);
			if (resultSet.size() != 0)
			{
				dependencyDB.Add_To_Children(file, resultSet);
			}
		}
	}
	if (file.find(".h") != std::string::npos)
	{
		auto & h_cpp_table = table.Get_H_CPP_dep();
		if (h_cpp_table.find(file) != h_cpp_table.end())
		{
			dependencyDB[file].children.getRef().insert(h_cpp_table[file]);
		}
	}

}

void DependencyAnalyzer::AddNewFile(const std::string & key, const std::string & category, const std::string & description)
{
	dependencyDB.Add_entry(key, category, description, "File data");
}

NoSqlDb<std::string>& DependencyAnalyzer::GetAnalyResult()
{
	return dependencyDB;
}

void DependencyAnalyzer::SaveAnalyResultToFile(const std::string & path)
{
	if (dependencyDB.Serialize(path))
	{
		std::cout << "\n\nDependency analysis result saved to " << path << std::endl;
		std::cout << "\n\nDisplaying XML content:" << std::endl;
		std::cout << "\n" << dependencyDB.ToXml() << std::endl;
	}
	else
		std::cout << "\n\nFailed opening " << "Dependency analysis result not saved." << std::endl;
}

void DependencyAnalyzer::DisplayTypeTable()
{
	table.Display();
}

void DependencyAnalyzer::DisplayDependency()
{
	std::cout << "\n\n\n" << "Demonstrating dependency information:" << std::endl;

	for (auto & elem : dependencyDB)
	{
		std::cout <<"\n\n"<< "=================================================" << std::endl;
		std::cout << "File:        " << elem.first << std::endl;
		std::cout << "Dependency:" << std::endl;
		for (auto & children : elem.second.children.getRef())
			std::cout << children << std::endl;
	}
}


#ifdef DEBUG_DEF



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
	//ast.add(pyc1);                                       // add c1 scope
	//ast.pop();                                          // end c1 scope
	//ast.pop();                                          // end f1 scope
	//ASTNode* pyf2 = new ASTNode("function", "f2");
	//ast.add(pyf2);                                       // add f2 scope
	//ast.pop();                                          // end f2 scope
	//ast.pop();                                         // end X scope
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
#endif // DEBUG_DEF