// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

export module UnitTests_Graph;

import jpt.Graph;
import jpt.DynamicArray;
import jpt.TypeDefs;

class Foo
{
public:
	char m_data = '0';

	Foo() = default;
	Foo(char data)
		: m_data(data)
	{
	}
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
	Index handleC = graph.AddNode(C);
	Index handleD = graph.AddNode(D);
	Index handleE = graph.AddNode(E);
	Index handleF = graph.AddNode(F);
	Index handleG = graph.AddNode(G);
	//Index handleH = graph.AddNode(H);
	graph.AddNode(H);
	Index handleI = graph.AddNode(I);
	Index handleJ = graph.AddNode(J);

	// Add edges
	graph.AddEdge(handleA, handleD, 1.0f);

	graph.AddEdge(handleB, handleA, 2.0f);
	graph.AddEdgeBoth(handleB, handleE, 2.0f);

	graph.AddEdgeBoth(handleC, handleF, 5.0f);
	graph.AddEdgeBoth(handleC, handleI, 8.0f);

	graph.AddEdge(handleD, handleE, 4.0f);
	graph.AddEdge(handleD, handleF, 3.0f);
	graph.AddEdge(handleD, handleB, 2.0f);

	graph.AddEdgeBoth(handleE, handleF, 2.0f);
	graph.AddEdge(handleE, handleJ, 3.0f);

	graph.AddEdgeBoth(handleF, handleI, 10.0f);
	graph.AddEdge(handleF, handleG, 3.0f);

	graph.AddEdge(handleG, handleA, 15.0f);
	graph.AddEdge(handleG, handleD, 2.0f);

	// Erase node
	//graph.AddEdge(handleH, handleG, 1.0f);	// Dummy for not using handleH warning
	//graph.AddEdge(handleH, handleI, 1.0f);
	//graph.AddEdge(handleJ, handleH, 1.0f);
	//graph.AddEdge(handleA, handleH, 1.0f);
	//graph.AddEdge(handleC, handleH, 1.0f);
	//graph.EraseNode(handleH);

	return graph;
}

static bool DFS(const jpt::Graph<Foo>& graph)
{
	Index i = graph.FindIndex('F');

	graph.DFS(i, [](const Foo& foo)
		{
			JPT_LOG(foo.m_data);
		});

	return true;
}

static bool BFS(const jpt::Graph<Foo>& graph)
{
	Index i = graph.FindIndex('F');

	graph.BFS(i, [](const Foo& foo)
		{
			JPT_LOG(foo.m_data);
		});

	return true;
}

export bool RunUnitTests_Graph()
{
	jpt::Graph<Foo> graph = GetGraph();

	DFS(graph);
	JPT_LOG("---------------------------");
	BFS(graph);

	return true;
}