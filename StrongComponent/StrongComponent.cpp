/////////////////////////////////////////////////////////////////////////////
//  StrongComponet.cpp - Analyze strongly connected components in a graph  //
//  Language:     C++, VS 2015                                             //
//  Platform:     SurfaceBook, Windows 10 Pro                              //
//  Application:  Project1 for CSE687 - Object Oriented Design             //
//  Author:       Weijun Cai                                               //
/////////////////////////////////////////////////////////////////////////////


#include "StrongComponent.h"



void StrongComponents::BiuldGraph(NoSqlDb<std::string>& depDB)
{
	std::unordered_map<Vertex*, std::set<std::string>> parent_children_map;
	std::unordered_map<std::string, size_t> idMap;
	for (auto & DBelement : depDB)
	{
		std::string filename = DBelement.first;
		Vertex* vertexToAdd = new Vertex(filename);
		parent_children_map[vertexToAdd] = DBelement.second.children;
		idMap[filename] = vertexToAdd->id();
		depGraph.addVertex(*vertexToAdd);
	}

	for (auto & parent_children_pair : parent_children_map)
	{
		auto parent = *parent_children_pair.first;
		for (auto & child : parent_children_pair.second)
			depGraph.addEdge("", parent, depGraph[depGraph.findVertexIndexById(idMap[child])]);
	}

	for (auto & _pair : parent_children_map)
		delete _pair.first;
}

void StrongComponents::AnalyzeSCCs()
{
	size_t index = 0;
	std::stack<Vertex> s;

	for (auto & v : depGraph)
	{
		if (v.Index() == -1)
			StrongConnect(v, s, index);
	}
}

void StrongComponents::DisplaySCS()
{
	std::cout << "\n\n\nDemonstrating strongly connected components:" << std::endl;
	for (auto & sc : SCCset)
	{
		std::cout << "\n\n====================================" << std::endl;
		std::cout << "Strong component:" << std::endl;
		for (auto & elem : sc)
		{
			std::cout << elem << std::endl;
		}
	}
}

void StrongComponents::StrongConnect(Vertex & vertex, std::stack<Vertex>& stack, size_t & index)
{
	vertex.Index() = (int)index;
	vertex.lowlink() = (int)index;
	++index;
	stack.push(vertex);
	vertex.OnStack() = true;

	for (auto child : vertex.Edges())
	{
		auto & childV = depGraph[depGraph.findVertexIndexById(child.first)];
		if (childV.Index() == -1)
		{
			StrongConnect(childV, stack, index);
			vertex.lowlink() = std::min(vertex.lowlink(), childV.lowlink());
		}
		else if (childV.OnStack())
		{
			vertex.lowlink() = std::min(vertex.lowlink(), childV.Index());
		}
	}

	if (vertex.lowlink() == vertex.Index())
	{
		std::unordered_set<std::string> scc;
		Vertex w("");
		while (stack.top().id() != vertex.id())
		{
			w = stack.top();
			depGraph[depGraph.findVertexIndexById(w.id())].OnStack() = false;
			//stack.top().OnStack() = false;
			stack.pop();
			scc.insert(w.value());
		}
		w = stack.top();
		scc.insert(w.value());
		SCCset.push_back(std::move(scc));
		depGraph[depGraph.findVertexIndexById(w.id())].OnStack() = false;
		//stack.top().OnStack() = false;
		stack.pop();
	}
}


#ifdef STRONGCOMP




void main()
{
	NoSqlDb<std::string> db;
	Element<std::string> elem1;
	elem1.name = "elem1";
	elem1.category = "test";
	elem1.description = "desc1";
	elem1.timeDate = "Jan 01 2017 12:10:8";
	elem1.data = "elem1's StrData";
	elem1.children.getRef().insert("elem2");
	elem1.children.getRef().insert("elem3");
	db.Add_entry(elem1.name, elem1); //name is the key?
	Element<std::string> elem2;
	elem2.name = "elem2";
	elem2.category = "test";
	elem2.description = "desc2";
	elem2.timeDate = "Jan 02 2017 10:10:6";
	elem2.children.getRef().insert("elem3");
	elem2.data = "elem2's StrData";
	db.Add_entry(elem2.name, elem2);
	Element<std::string> elem3;
	elem3.name = "elem3";
	elem3.category = "test";
	elem3.description = "desc3";
	elem3.timeDate = "Jan 02 2017 10:10:6";
	elem3.data = "elem3's StrData";
	db.Add_entry(elem3.name, elem3);
	Element<std::string> elem4;
	elem4.name = "elem4";
	elem4.category = "test";
	elem4.description = "desc3";
	elem4.timeDate = "Jan 02 2017 10:10:6";
	elem4.children.getRef().insert("elem3");
	elem4.data = "elem3's StrData";
	db.Add_entry(elem4.name, elem4);
	Element<std::string> elem5;
	elem5.name = "elem5";
	elem5.category = "test";
	elem5.description = "desc3";
	elem5.timeDate = "Jan 02 2017 10:10:6";
	elem5.children.getRef().insert("elem2");
	elem5.data = "elem5's StrData";
	db.Add_entry(elem5.name, elem5);
	StrongComponents sc;
	sc.BiuldGraph(db);
	sc.AnalyzeSCCs();
	sc.DisplaySCS();
	system("pause");
}
#endif // STRONGCOMP