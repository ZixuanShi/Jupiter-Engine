// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Vulkan.Utils;

import jpt.Vulkan.Vertex;
import jpt.Vector2;
import jpt.Vector3;

export namespace jpt::Vulkan
{
	void GenerateSierpinski(size_t depth, Vec2f right, Vec2f left, Vec2f top)
	{
		if (depth <= 0)
		{
			static const Vec3f klocTriangleColor = { 0.95f, 0.05f, 0.05f };
			const size_t baseIndex = g_vertices.Count();

			g_vertices.EmplaceBack(Vertex(right, klocTriangleColor));
			g_vertices.EmplaceBack(Vertex(left, klocTriangleColor));
			g_vertices.EmplaceBack(Vertex(top, klocTriangleColor));

			g_indices.EmplaceBack(static_cast<uint16>(baseIndex));     // right
			g_indices.EmplaceBack(static_cast<uint16>(baseIndex + 1)); // left  
			g_indices.EmplaceBack(static_cast<uint16>(baseIndex + 2)); // top

			return;
		}

		const Vec2f rightLeft = (right + left) * 0.5f;
		const Vec2f rightTop = (right + top) * 0.5f;
		const Vec2f leftTop = (left + top) * 0.5f;

		GenerateSierpinski(depth - 1, right, rightLeft, rightTop);
		GenerateSierpinski(depth - 1, rightLeft, left, leftTop);
		GenerateSierpinski(depth - 1, rightTop, leftTop, top);
	}
}