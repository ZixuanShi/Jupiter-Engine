module;

#include <limits>

export module jpt.NumericLimits;

namespace jpt
{
	export template<typename T>
	struct LimitsOf
	{
		static constexpr T kMax = std::numeric_limits<T>::max();
		static constexpr T kMin = std::numeric_limits<T>::min();
	};
}