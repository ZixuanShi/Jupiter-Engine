// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_MathPractice_MergeCloseVertices;

import jpt.CoreModules;

static constexpr size_t kCount = 10;

void MergeCloseVertices(jpt::DynamicArray<Vec3f>& vertices, float32)
{
	jpt::Sort(vertices, [](const Vec3f& a, const Vec3f& b)
	{
		return a.Length2() < b.Length2();
	});
}

export bool UnitTest_MathPractice_MergeCloseVertices()
{
	jpt::DynamicArray<Vec3f> vertices;
	vertices.Reserve(kCount);
	vertices.EmplaceBack(    5.0f, 5.01f, 5.0f);
	vertices.EmplaceBack(    5.0f, -5.01f, 5.0f);
	vertices.EmplaceBack(   9.0f, 9.01f, 5.01f);
	vertices.EmplaceBack(   1.0f,  2.0f,  5.0f);
	vertices.EmplaceBack(   1.0f,  2.0f,  -5.0f);
	vertices.EmplaceBack(  10.0f, 5.01f,  2.0f);
	vertices.EmplaceBack(   8.0f,  5.0f, 2.01f);
	vertices.EmplaceBack(   1.0f, 2.01f,  5.0f);
	vertices.EmplaceBack(  10.0f,  5.0f,  2.0f);
	vertices.EmplaceBack( 8.025f,  5.0f,  2.0f);
	vertices.EmplaceBack(   5.0f,  5.0f,  5.0f);

	for (Vec3f v : vertices)
	{
		JPT_LOG(v);
	}

	JPT_LOG("after---------------------");
	MergeCloseVertices(vertices, 0.01f);

	for (Vec3f v : vertices)
	{
		JPT_LOG(v);
	}

	return true;
}