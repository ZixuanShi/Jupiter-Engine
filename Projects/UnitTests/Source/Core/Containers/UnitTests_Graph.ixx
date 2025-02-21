// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

export module UnitTests_Graph;

import jpt.Graph;
import jpt.DynamicArray;
import jpt.TypeDefs;
import jpt.Optional;

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

// Projects/UnitTests/Assets/UnitTests_Graph_0.jpg
export jpt::Graph<Foo> GetGraph0()
{
	jpt::Graph<Foo> graph;

	const Foo A('A');
	const Foo B('B');
	const Foo C('C');
	const Foo D('D');
	const Foo E('E');
	const Foo F('F');
	const Foo G('G');
	const Foo H('H');
	const Foo I('I');
	const Foo J('J');

	// Add nodes
	const Index handleA = graph.AddNode(A);
	const Index handleB = graph.AddNode(B);
	const Index handleC = graph.AddNode(C);
	const Index handleD = graph.AddNode(D);
	const Index handleE = graph.AddNode(E);
	const Index handleF = graph.AddNode(F);
	const Index handleG = graph.AddNode(G);
	//Index handleH = graph.AddNode(H);
	graph.AddNode(H);
	const Index handleI = graph.AddNode(I);
	const Index handleJ = graph.AddNode(J);

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
	//graph.AddEdge(handleH, handleG, 1.0f);
	//graph.AddEdge(handleH, handleI, 1.0f);
	//graph.AddEdge(handleJ, handleH, 1.0f);
	//graph.AddEdge(handleA, handleH, 1.0f);
	//graph.AddEdge(handleC, handleH, 1.0f);
	//graph.EraseNode(handleH);

	return graph;
}

// Projects/UnitTests/Assets/UnitTests_Graph_1.jpg
export jpt::Graph<Foo> GetGraph1()
{
	jpt::Graph<Foo> graph;

	const Foo A('A');
	const Foo B('B');
	const Foo C('C');
	const Foo D('D');
	const Foo E('E');
	const Foo F('F');
	const Foo G('G');
	const Foo H('H');
	const Foo I('I');

	// Add nodes
	const Index handleA = graph.AddNode(A);
	const Index handleB = graph.AddNode(B);
	const Index handleC = graph.AddNode(C);
	const Index handleD = graph.AddNode(D);
	const Index handleE = graph.AddNode(E);
	const Index handleF = graph.AddNode(F);
	const Index handleG = graph.AddNode(G);
	const Index handleH = graph.AddNode(H);
	const Index handleI = graph.AddNode(I);

	// Add edges
	graph.AddEdge(handleA, handleB);
	graph.AddEdge(handleA, handleC);
	graph.AddEdge(handleA, handleD);
	graph.AddEdge(handleA, handleE);
	graph.AddEdge(handleA, handleF);

	graph.AddEdge(handleE, handleG);
	graph.AddEdge(handleG, handleH);

	graph.AddEdge(handleC, handleI);

	return graph;
}

// Projects/UnitTests/Assets/UnitTests_Graph_1.jpg
export jpt::Graph<Foo> GetGraph2()
{
	jpt::Graph<Foo> graph;

	const Foo A('A');
	const Foo B('B');
	const Foo C('C');
	const Foo D('D');
	const Foo E('E');
	const Foo F('F');
	const Foo G('G');
	const Foo H('H');
	const Foo I('I');
	const Foo J('J');
	const Foo K('K');
	const Foo L('L');
	const Foo M('M');

	// Add nodes
	const Index handleA = graph.AddNode(A);
	const Index handleB = graph.AddNode(B);
	const Index handleC = graph.AddNode(C);
	const Index handleD = graph.AddNode(D);
	const Index handleE = graph.AddNode(E);
	const Index handleF = graph.AddNode(F);
	const Index handleG = graph.AddNode(G);
	const Index handleH = graph.AddNode(H);
	const Index handleI = graph.AddNode(I);
	const Index handleJ = graph.AddNode(J);
	const Index handleK = graph.AddNode(K);
	const Index handleL = graph.AddNode(L);
	const Index handleM = graph.AddNode(M);

	// Add edges
	graph.AddEdgeBoth(handleA, handleB);
	graph.AddEdgeBoth(handleA, handleC);
	graph.AddEdgeBoth(handleA, handleD);
	graph.AddEdgeBoth(handleA, handleE);

	graph.AddEdgeBoth(handleB, handleF);
	graph.AddEdgeBoth(handleB, handleG);

	graph.AddEdgeBoth(handleC, handleH);
	graph.AddEdgeBoth(handleC, handleI);

	graph.AddEdgeBoth(handleD, handleJ);
	graph.AddEdgeBoth(handleD, handleK);

	graph.AddEdgeBoth(handleE, handleL);
	graph.AddEdgeBoth(handleE, handleM);

	return graph;
}

export bool DFS(const jpt::Graph<Foo>& graph, const Foo& start)
{
	const Index i = graph.FindIndex(start);

	graph.DFS(i, [](const Foo& foo)
		{
			JPT_LOG(foo.m_data);
		});

	return true;
}

export bool BFS(const jpt::Graph<Foo>& graph, const Foo& start)
{
	const Index i = graph.FindIndex(start);

	graph.BFS(i, [](const Foo& foo)
		{
			JPT_LOG(foo.m_data);
		});

	return true;
}

export bool Dijkstra(const jpt::Graph<Foo>& graph, const Foo& start, const Foo& end)
{
	const Index handleStart = graph.FindIndex(start);
	const Index handleEnd = graph.FindIndex(end);

	const auto path = graph.Dijkstra(handleStart, handleEnd);
	if (path)
	{
		auto result = path.Value();
		for (Index i : result)
		{
			const Foo& foo = graph[i];
			JPT_LOG(foo.m_data);
		}
	}

	return true;
}

bool Dijkstra()
{
	// Dijsktra
	jpt::Graph<Foo> graph;
	jpt::Optional<jpt::DynamicArray<Index>> path;
	jpt::DynamicArray<Index> expected;

	// Graph 0
	graph    = GetGraph0();
	path     = graph.Dijkstra(graph.FindIndex('A'), graph.FindIndex('C'));
	expected = { graph.FindIndex('A'), graph.FindIndex('D'), graph.FindIndex('F'), graph.FindIndex('C') };
	JPT_ENSURE(path.Value() == expected);

	path     = graph.Dijkstra(graph.FindIndex('I'), graph.FindIndex('A'));
	expected = { graph.FindIndex('I'), graph.FindIndex('F'), graph.FindIndex('E'), graph.FindIndex('B'), graph.FindIndex('A') };
	JPT_ENSURE(path.Value() == expected);

	// Graph 1
	graph    = GetGraph1();
	path     = graph.Dijkstra(graph.FindIndex('A'), graph.FindIndex('H'));
	expected = { graph.FindIndex('A'), graph.FindIndex('E'), graph.FindIndex('G'), graph.FindIndex('H') };
	JPT_ENSURE(path.Value() == expected);

	path     = graph.Dijkstra(graph.FindIndex('H'), graph.FindIndex('A'));
	expected = {};
	JPT_ENSURE(!path.HasValue());

	// Graph 2
	graph    = GetGraph2();
	path     = graph.Dijkstra(graph.FindIndex('J'), graph.FindIndex('H'));
	expected = { graph.FindIndex('J'), graph.FindIndex('D'), graph.FindIndex('A'), graph.FindIndex('C'), graph.FindIndex('H') };
	JPT_ENSURE(path.Value() == expected);

	path     = graph.Dijkstra(graph.FindIndex('A'), graph.FindIndex('M'));
	expected = { graph.FindIndex('A'), graph.FindIndex('E'), graph.FindIndex('M') };
	JPT_ENSURE(path.Value() == expected);

	return true;
}

export bool RunUnitTests_Graph()
{
	//jpt::Graph<Foo> graph = GetGraph2();

	//DFS(graph, 'A');
	//JPT_LOG("---------------------------");
	//BFS(graph, 'A');
	//JPT_LOG("---------------------------");

	JPT_ENSURE(Dijkstra());

	return true;
}