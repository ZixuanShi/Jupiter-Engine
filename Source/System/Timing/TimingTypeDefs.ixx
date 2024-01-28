// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include <chrono>

export module jpt.Timing.TypeDefs;

export namespace jpt
{
	namespace Timing
	{
		// Concepts
		template<class T>
		concept ClockT = std::chrono::is_clock_v<T>;
	
		// TypeDefs
		using Precision = double;
		using Duration  = std::chrono::duration<Precision>; /**< Duration between two TimePoints */
		using StopWatch = std::chrono::steady_clock;		/**< Stopwatch that will only grow positively. Use for timing and benchmarking. */
		using Clock     = std::chrono::system_clock;		/**< Regular system-wide realtime clock. Use for simulating real-life time like day/night shifting*/
		
		using HighResolutionClock = std::chrono::high_resolution_clock;	/**< The clock with the shortest tick period and highest precision available. */
	
		/**< Represents a point in time. It is implemented as if it stores a value of T Duration indicating the time interval from the start of the Clock's epoch. */
		template<ClockT T>
		using Point = std::chrono::time_point<T>;
	}
}