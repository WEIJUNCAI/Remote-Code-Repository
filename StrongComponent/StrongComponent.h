#pragma once

/////////////////////////////////////////////////////////////////////////////
//  StrongComponet.h - Analyze strongly connected components in a graph    //
//  Language:     C++, VS 2015                                             //
//  Platform:     SurfaceBook, Windows 10 Pro                              //
//  Application:  Project1 for CSE687 - Object Oriented Design             //
//  Author:       Weijun Cai                                               //
/////////////////////////////////////////////////////////////////////////////
/*
*   Module Operations
*   -----------------
*   This module contains the delaration of StrongComponets class, which
*   analyze the strongly connencted components given a graph. The graph is 
*   first built from a NoSqlDb instance.
*/
/*
*   Build Process
*   -------------
*   - Required files: Graph.h, Graph.cpp
*                     NoSqlDb.h
*                     StrongComponents.h, StrongComponents.cpp
*
*   Maintenance History
*   -------------------
*   ver 1.0 : 22 Feb 2017
*/


#include"../Graph/Graph/Graph.h"
#include"../DependencyAnalyzer/DependencyAnalyzer.h"



class StrongComponents
{
public:
	using Vertex = GraphLib::Vertex<std::string, std::string>;
	
	// construct a graph from specified NoSqlDb
	void BiuldGraph(NoSqlDb<std::string>& depDB);

	// analyze strongly connected components on a graph
	void AnalyzeSCCs();

	// display the strongly connected components analysis result.
	void DisplaySCS();
private:
	// travers a graph to analyze SCCs using Tarjan algoritm
	void StrongConnect(Vertex& vertex, std::stack<Vertex>& stack, size_t& index);


	GraphLib::Graph<std::string, std::string> depGraph;
	std::vector<std::unordered_set<std::string>> SCCset;

};