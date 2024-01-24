module;

#include <limits>

export module jpt.NumericLimits;

namespace jpt
{
	export template<typename T>
	struct NumericLimits
	{
		T Max() { return std::numeric_limits<T>::max(); }
		T Min() { return std::numeric_limits<T>::min(); }
	};
}