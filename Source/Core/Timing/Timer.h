// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

namespace jpt
{
	/** Gets a duration of time */
	class JPT_API Timer
	{	
	public:
		/** Restarts the timer */
		void Start();

		/** @return The time duration from now to last time we called Start() */
		double GetTimeDuration();
	};
}