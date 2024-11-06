// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"

export module UnitTests_Graph;

import jpt.Graph;
import jpt.TypeDefs;

class Foo
{
public:
	char m_data = '0';
};

jpt::Graph<Foo> GetGraph()
{
	jpt::Graph<Foo> graph;

	Foo foo0('A');
	Foo foo1('B');
	Foo foo2('C');
	Foo foo3('D');
	Foo foo4('E');
	Foo foo5('F');
	Foo foo6('G');
	Foo foo7('H');
	Foo foo8('I');
	Foo foo9('J');

	graph.AddNode(foo0);
	graph.AddNode(foo1);
	graph.AddNode(foo2);
	graph.AddNode(foo3);
	graph.AddNode(foo4);
	graph.AddNode(foo5);
	graph.AddNode(foo6);
	graph.AddNode(foo7);
	graph.AddNode(foo8);
	graph.AddNode(foo9);

	graph.AddEdge(foo0, foo1, 1.0f);

	return graph;
}

export bool RunUnitTests_Graph()
{


	return true;
}