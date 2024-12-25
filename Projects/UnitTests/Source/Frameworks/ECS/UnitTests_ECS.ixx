// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

export module UnitTests_ECS;

import jpt.Scene;
import jpt.Entity;
import jpt.Entity.Component;
import jpt.Entity.Factory;

import jpt.Vector3;
import jpt.Matrix44;
import jpt.Rand;
import jpt.Time.TypeDefs;
import jpt.Math;

//------------------------------------------------------------------------
// Data
//------------------------------------------------------------------------
class EntityComponent_1 final : public jpt::EntityComponent
{
private:
	Matrix44f m_transform = Matrix44f::Identity();

public:
	virtual bool Init() override
	{
		m_transform.RotateX(jpt::ToRadians(90.0f));
		return true;
	}

	const Matrix44f& GetTransform() const { return m_transform; }
};

class UpdatableEntityComponent_1 final : public jpt::UpdatableEntityComponent
{
private:
	int m_data = 0;

public:
	virtual void Update(jpt::TimePrecision) override
	{
		++m_data;
	}

	int GetData() const { return m_data; }
};

//------------------------------------------------------------------------
// Test
//------------------------------------------------------------------------
static bool Entities()
{
	jpt::Entity entity1;

	entity1.AddComponent<EntityComponent_1>();
	entity1.AddComponent<UpdatableEntityComponent_1>();

	JPT_ENSURE(entity1.GetComponent<EntityComponent_1>());
	JPT_ENSURE(entity1.GetComponent<UpdatableEntityComponent_1>());

	EntityComponent_1* entityComponent1 = entity1.GetComponent<EntityComponent_1>();
	JPT_ENSURE(entityComponent1->GetTransform().ToEulerDegrees().x == 90.0f);

	UpdatableEntityComponent_1* updatableEntityComponent1 = entity1.GetComponent<UpdatableEntityComponent_1>();
	JPT_ENSURE(updatableEntityComponent1->GetData() == 0);

	for (int i = 0; i < 10; ++i)
	{
		entity1.Update(0.0f);
	}

	JPT_ENSURE(updatableEntityComponent1->GetData() == 10);

	entity1.Shutdown();

	return true;
}

export bool RunUnitTests_ECS()
{
	JPT_ENSURE(Entities());

	return true;
}