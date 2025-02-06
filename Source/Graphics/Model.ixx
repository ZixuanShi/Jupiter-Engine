// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Model;

import jpt.Mesh;

import jpt.DynamicArray;

export namespace jpt
{
	/** 3D Model in world. Collection of 
		- Meshes: Form its geometry shape
		- Textures: Form its appearance
		- Materials: Form its physical properties
		- Animations: Form its movement
		- Skeleton: Form its bone structure
		- Physics: Form its collision detection
		- LODs: Form its level of detail
		- Occlusion Culling: Form its visibility
		- Ray Tracing: Form its light interaction */
	class Model
	{
	private:
		DynamicArray<Mesh> m_meshes;
	};
}