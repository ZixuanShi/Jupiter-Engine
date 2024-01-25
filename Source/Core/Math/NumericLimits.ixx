module;

#include <limits>

export module jpt.NumericLimits;

namespace jpt
{
	export template<typename T>
	struct NumericLimits
	{
		static constexpr T Max() { return std::numeric_limits<T>::max(); }
		static constexpr T Min() { return std::numeric_limits<T>::min(); }
	};
}