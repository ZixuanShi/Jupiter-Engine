// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

namespace jpt
{
	/** Base abstract class for applications.
		It holds window, renderer, audio, collision managers, etc.*/
	class ApplicationBase
	{
	public:
		virtual ~ApplicationBase() = default;

		virtual bool Init();
		virtual void Update();
		virtual void Clean();

		/** Designed to be implemented in client game C++
			@return An Application's pointer to run the game */
		static ApplicationBase& GetInstance();
	};
}
