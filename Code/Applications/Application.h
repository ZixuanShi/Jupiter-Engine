#pragma once
#include "Core/Building/Macros.h"

namespace jpt
{
// Base class for applications
class JPT_API Application
{
public:
	virtual ~Application() = default;

public:
	virtual bool Init();
	virtual void Update();
	virtual void Clean();

	static Application* GetInstance() { return s_instance; }

private:
	static Application* s_instance;
};

Application* CreateApplication();
}