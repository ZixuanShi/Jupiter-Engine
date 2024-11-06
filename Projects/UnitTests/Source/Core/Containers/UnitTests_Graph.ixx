// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"

export module UnitTests_Graph;

import jpt.Graph;
import jpt.DynamicArray;
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
	Index handleC = graph.AddNode(C);
	Index handleD = graph.AddNode(D);
	Index handleE = graph.AddNode(E);
	Index handleF = graph.AddNode(F);
	Index handleG = graph.AddNode(G);
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

	return graph;
}

struct Test
{
	Index m_id = 0;
	char m_data;
};

struct Bar
{
	struct Handle
	{
		Index* m_pId = nullptr;
	};

	jpt::DynamicArray<Test> m_array;

	Handle Add(char data)
	{
		Test test;
		test.m_data = data;
		test.m_id = m_array.Count();
		m_array.EmplaceBack(test);

		Handle handle;
		handle.m_pId = &m_array[test.m_id].m_id;
		return handle;
	}

	void Erase(Handle handle)
	{
		m_array.Erase(*handle.m_pId);

		for (Index i = *handle.m_pId; i < m_array.Count(); ++i)
		{
			m_array[i].m_id = i;
		}
	}
};

export bool RunUnitTests_Graph()
{
	GetGraph();

	Bar bar;
	Bar::Handle handleA = bar.Add('A');
	Bar::Handle handleB = bar.Add('B');
	Bar::Handle handleB2 = bar.Add('B');
	Bar::Handle handleC = bar.Add('C');
	Bar::Handle handleD = bar.Add('D');

	bar.Erase(handleB);

	return true;
}