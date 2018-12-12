#pragma once
#include <limits>
#include <utility/Literals.h>

// someone defined a max macro....
// lets undefine such horrors
#undef max

template<class T>
T checked_cast(size_t value)
{
	if (static_cast<size_t>(std::numeric_limits<T>::max()) < value)
	{
		throw std::overflow_error("value: '"_s + std::to_string(value) + "' is too large to be converted to JS types");
	}
	return static_cast<T>(value);
}