// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Debugging/Assert.h"

#include <time.h>
#include <limits>

export module jpt.RandomNumberGenerator;

import jpt.TypeDefs;
import jpt.Concepts;

export namespace jpt
{
	/** Random number generator using xorshift128+ */
	class RNG
	{
	private:
		uint64 m_seeds[2];

	public:
		RNG(uint64 seed = unsigned(time(nullptr)));

		/** @return		a random number from min to max inclusive
			@param min	Minimum value inclusive
			@param max	Maximum value inclusive	*/
		template<Integral IntT = uint32>
		IntT RandInRange(IntT min, IntT max);

		/** @return		a random integral value within max range
			@param max	Largest possible value (inclusive) */
		template<Integral IntT = uint32>
		IntT RandInMax(IntT max = (std::numeric_limits<IntT>::max)());

		/** @return		a random floating point from 0 to 1 inclusive */
		template<Floating Type = float>
		Type FRand();

		/** @return		a random floating point from min to max inclusive
			@param min	Minimum value inclusive
			@param max	Maximum value inclusive	*/
		template<Floating Type = float>
		Type FRand(Type min, Type max);

		/** @return		a random boolean */
		bool RandBool();

		/** Update two seeds
			@param Seed	The seed as input */
		void SetSeed(uint64 seed);

		/** @return a static global rng */
		static RNG& Global();

	private:
		/** xorshift128+ algorithm from https://en.wikipedia.org/wiki/Xorshift
			@template Type	The result will be in this Type	 */
		template<Integral IntT>
		IntT XOrShift128Plus();
	};

	RNG::RNG(uint64 seed /* = unsigned(time(nullptr))*/)
	{
		m_seeds[0] = seed << 3 | seed;
		m_seeds[1] = seed << 7 | seed;
	}

	template<Floating Type>
	Type RNG::FRand()
	{
		float result = 0.f;

		// mangling...
		const uint64 temp1 = m_seeds[1];
		m_seeds[0] ^= m_seeds[0] << 23;
		const uint64 temp2 = m_seeds[0] ^ m_seeds[1] ^ (m_seeds[0] >> 17) ^ (m_seeds[1] >> 26);
		const uint64 temp3 = temp2 + m_seeds[1];

		result = 1.0f / ((float)INT_MAX + 1.0f) * (0x7FFFFFFF & temp3);

		// reassign seed
		m_seeds[0] = temp1;
		m_seeds[1] = temp2;

		return result;
	}

	template<Floating Type>
	Type RNG::FRand(Type min, Type max)
	{
		const Type randomFloat = FRand();
		const Type range = max - min;
		return randomFloat * range + min;
	}

	bool RNG::RandBool()
	{
		return static_cast<bool>(RandInRange(0, 1));
	}

	void RNG::SetSeed(uint64 seed)
	{
		JPT_ASSERT(seed != 0, "seed should not be zero");
		m_seeds[0] = seed << 3 | seed;
		m_seeds[1] = seed << 7 | seed;
	}

	RNG& RNG::Global()
	{
		static RNG globalRng;
		return globalRng;
	}

	template<Integral IntT>
	IntT RNG::RandInRange(IntT min, IntT max)
	{
		uint64 mod = (uint64)max - (uint64)min + 1;
		return XOrShift128Plus<IntT>() % mod + min;
	}

	template<Integral IntT>
	IntT RNG::RandInMax(IntT max /* = (std::numeric_limits<IntT>::max)()*/)
	{
		return RandInRange<IntT>(0, max);
	}

	template<Integral IntT>
	IntT RNG::XOrShift128Plus()
	{
		uint64 T = m_seeds[0];
		const uint64 S = m_seeds[1];

		m_seeds[0] = S;

		T ^= T << 23;		// a
		T ^= T >> 17;		// b -- Again, the shifts and the multipliers are tunable
		T ^= S ^ (S >> 26);	// c

		m_seeds[1] = T;
		return IntT(T + S);
	}
}