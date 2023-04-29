#pragma once

namespace jpt
{
	class JPT_API Timer
	{	
	public:
		// Starts the timer
		void Start();

		// Get the time duration from now to when we started the timer
		double GetTimeDuration();
	};
}