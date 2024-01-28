// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

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
		constexpr RNG(uint64 seed = unsigned(time(nullptr)));

		/** @return		a random number from min to max inclusive
			@param min	Minimum value inclusive
			@param max	Maximum value inclusive	*/
		template<Integral TInt = uint32>
		constexpr TInt RandInRange(TInt min, TInt max);

		/** @return		a random integral value within max range
			@param max	Largest possible value (inclusive) */
		template<Integral TInt = uint32>
		constexpr TInt RandInMax(TInt max = (std::numeric_limits<TInt>::max)());

		/** @return		a random floating point from 0 to 1 inclusive */
		template<Floating Type = float>
		constexpr Type FRand();

		/** @return		a random floating point from min to max inclusive
			@param min	Minimum value inclusive
			@param max	Maximum value inclusive	*/
		template<Floating Type = float>
		constexpr Type FRand(Type min, Type max);

		/** @return		a random boolean */
		constexpr bool RandBool();

		/** Update two seeds
			@param Seed	The seed as input */
		constexpr void SetSeed(uint64 seed);

		/** @return a static global rng */
		static RNG& Global();

	private:
		/** xorshift128+ algorithm from https://en.wikipedia.org/wiki/Xorshift
			@template Type	The result will be in this Type	 */
		template<Integral TInt>
		constexpr TInt XOrShift128Plus();
	};

	constexpr RNG::RNG(uint64 seed /* = unsigned(time(nullptr))*/)
	{
		m_seeds[0] = seed << 3 | seed;
		m_seeds[1] = seed << 7 | seed;
	}

	template<Floating Type>
	constexpr Type RNG::FRand()
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

	template<Floating T>
	constexpr T RNG::FRand(T min, T max)
	{
		const T randomFloat = FRand();
		const T range = max - min;
		return randomFloat * range + min;
	}

	constexpr bool RNG::RandBool()
	{
		return static_cast<bool>(RandInRange(0, 1));
	}

	constexpr void RNG::SetSeed(uint64 seed)
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

	template<Integral TInt>
	constexpr TInt RNG::RandInRange(TInt min, TInt max)
	{
		uint64 mod = (uint64)max - (uint64)min + 1;
		return XOrShift128Plus<TInt>() % mod + min;
	}

	template<Integral TInt>
	constexpr TInt RNG::RandInMax(TInt max /* = (std::numeric_limits<TInt>::max)()*/)
	{
		return RandInRange<TInt>(0, max);
	}

	template<Integral TInt>
	constexpr TInt RNG::XOrShift128Plus()
	{
		uint64 T = m_seeds[0];
		const uint64 S = m_seeds[1];

		m_seeds[0] = S;

		T ^= T << 23;		// a
		T ^= T >> 17;		// b -- Again, the shifts and the multipliers are tunable
		T ^= S ^ (S >> 26);	// c

		m_seeds[1] = T;
		return TInt(T + S);
	}
}