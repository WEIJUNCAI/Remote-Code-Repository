/////////////////////////////////////////////////////////////////////////////
//  TypeTable.cpp - Containner for type analysis                           //
//  Language:     C++, VS 2015                                             //
//  Platform:     SurfaceBook, Windows 10 Pro                              //
//  Application:  Project1 for CSE687 - Object Oriented Design             //
//  Author:       Weijun Cai                                               //
/////////////////////////////////////////////////////////////////////////////

#include <sstream>
#include <iomanip>
#include "TypeTable.h"



TypeTable::TypeMap & TypeTable::Type_Table()
{
	return type_table;
}

TypeTable::Map & TypeTable::Using_Name_Table()
{
	return using_table;
}

TypeTable::Map & TypeTable::Namespace_Table()
{
	return namespace_table;
}

TypeTable::Map & TypeTable::Typedef_Table()
{
	return typedef_table;
}

TypeTable::Map & TypeTable::Global_Var_Table()
{
	return global_var_table;
}

TypeTable::FuncMap & TypeTable::Func_Table()
{
	return func_table;
}

TypeTable::Map & TypeTable::H_CPP_Table()
{
	return h_cpp_dep_table;
}

std::string TypeTable::show()
{
	std::ostringstream out;
	out << "\nDemonstrating contents of type table:" << std::endl;
	show_type(out);
	show_namespace(out);
	show_alias(out);
	show_typedef(out);
	show_func(out);
	show_global_var(out);
	return out.str();
}

void TypeTable::show_type(std::ostream & out)
{
	out << " \nType Definitions:" << std::endl;
	out << std::setw(100) << std::left << "File" << std::setw(40) << std::left << "Type Name" << std::endl;
	out << "=========================================================================================================================================" << std::endl;

	for (auto pair : type_table)
	{
		for (auto vecPair : pair.second)
			out << std::setw(100) << std::left << vecPair.second << std::setw(25) << pair.first << std::endl;
	}
	out << "\n\n";
	out << std::setw(100) << std::left << "File" << std::setw(40) << std::left << "Fully-Qualified Name" << std::endl;
	out << "=========================================================================================================================================" << std::endl;

	for (auto pair : type_table)
	{
		for (auto vecPair : pair.second)
			out << std::setw(100) << std::left << vecPair.second << std::setw(25) << vecPair.first << std::endl;
	}
}


void TypeTable::show_namespace(std::ostream & out)
{
	out << "\n\n  Using (namespace import):" << std::endl;
	out << std::setw(30) << std::left << "File" << std::setw(25) << "Namespace" << std::endl;
	out << "====================================================" << std::endl;

	for (auto & pair_ : namespace_table)
	{
		out << std::setw(30) << std::left << pair_.second << std::setw(20) << std::left << pair_.first << std::endl;
	}
}

void TypeTable::show_alias(std::ostream & out)
{
	out << "\n\n  Using (alias):" << std::endl;
	out << std::setw(30) << std::left << "File" << std::setw(25) << "Alias" << std::endl;
	out << "====================================================" << std::endl;

	for (auto & pair_ : using_table)
	{
		out << std::setw(30) << std::left << pair_.second << std::setw(20) << std::left << pair_.first << std::endl;
	}
}

void TypeTable::show_typedef(std::ostream & out)
{
	out << "\n\n  typedef (alias):" << std::endl;
	out << std::setw(30) << std::left << "File" << std::setw(25) << "Alias" << std::endl;
	out << "====================================================" << std::endl;

	for (auto & pair_ : typedef_table)
	{
		out << std::setw(30) << std::left << pair_.second << std::setw(20) << std::left << pair_.first << std::endl;
	}
}

void TypeTable::show_func(std::ostream & out)
{
	out << "\n\n  Global Functions:" << std::endl;
	out << std::setw(100) << std::left << "File" << std::setw(25) << "Function" << std::endl;
	out << "================================================================================================================" << std::endl;

	for (auto & pair_ : func_table)
	{
		for (auto & file : pair_.second)
			out << std::setw(100) << std::left << file << std::setw(70) << std::left << pair_.first << std::endl;
	}
}

void TypeTable::show_global_var(std::ostream & out)
{
	out << "\n\n  Global data:" << std::endl;
	out << std::setw(30) << std::left << "File" << std::setw(50) << "Data" << std::endl;
	out << "====================================================" << std::endl;

	for (auto & pair_ : global_var_table)
	{
		out << std::setw(30) << std::left << pair_.second << std::setw(20) << std::left << pair_.first << std::endl;
	}
}

#ifdef DEBUG



void main()
{

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

#endif // DEBUG