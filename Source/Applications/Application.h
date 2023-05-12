#pragma once

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
	};
}