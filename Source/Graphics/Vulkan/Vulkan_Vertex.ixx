// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Vulkan.Data;

import jpt.Vertex;
import jpt.DynamicArray;
import jpt.TypeDefs;

export namespace jpt::Vulkan
{
	DynamicArray<Vertex> g_vertices;
	DynamicArray<uint32> g_indices;
	
	//DynamicArray<Vertex> g_vertices =
	//{
	//	  // Position             // Color              // TexCoord
	//	{ { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
	//	{ {  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },
	//	{ {  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
	//	{ { -0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },

	//	{ { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
	//	{ {  0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },
	//	{ {  0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
	//	{ { -0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }
	//};

	//DynamicArray<uint32> g_indices =
	//{
	//	0, 1, 2, 
	//	2, 3, 0,
	//	4, 5, 6,
	//	6, 7, 4,
	//};
}