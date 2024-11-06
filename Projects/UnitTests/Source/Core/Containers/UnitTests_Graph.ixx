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

constexpr bool operator==(const Foo& lhs, const Foo& rhs)
{
	return lhs.m_data == rhs.m_data;
}

// Projects/UnitTests/Assets/UnitTests_Graph.png
jpt::Graph<Foo> GetGraph()
{
	jpt::Graph<Foo> graph;

	Foo A('A');
	Foo B('B');
	Foo C('C');
	Foo D('D');
	Foo E('E');
	Foo F('F');
	Foo G('G');
	Foo H('H');
	Foo I('I');
	Foo J('J');

	// Add nodes
	Index handleA = graph.AddNode(A);
	Index handleB = graph.AddNode(B);
	graph.AddNode(C);
	Index handleD = graph.AddNode(D);
	Index handleE = graph.AddNode(E);
	graph.AddNode(F);
	graph.AddNode(G);
	graph.AddNode(H);
	graph.AddNode(I);
	graph.AddNode(J);

	// Add edges
	graph.AddEdge(handleA, handleD, 1.0f);

	graph.AddEdge(B, A, 2.0f);

	graph.AddEdge(D, E, 4.0f);
	graph.AddEdge(D, F, 3.0f);
	graph.AddEdge(D, B, 2.0f);

	graph.AddEdgeBoth(E, F, 2.0f);
	graph.AddEdge(E, J, 3.0f);
	graph.AddEdgeBoth(handleE, handleB, 2.0f);

	graph.AddEdgeBoth(F, I, 10.0f);
	graph.AddEdgeBoth(F, C, 5.0f);
	graph.AddEdge(F, G, 3.0f);

	graph.AddEdge(G, A, 15.0f);
	graph.AddEdge(G, D, 2.0f);

	graph.AddEdgeBoth(I, C, 8.0f);

	return graph;
}

export bool RunUnitTests_Graph()
{
	GetGraph();

	return true;
}