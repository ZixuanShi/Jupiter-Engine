module;

#include <limits>

export module jpt.NumericLimits;

namespace jpt
{
	export template<typename T>
	struct NumericLimits
	{
		T Max() const { return std::numeric_limits<T>::max(); }
		T Min() const { return std::numeric_limits<T>::min(); }
	};
}