#include <utility/Literals.h>

std::string operator "" _s(const char* s, size_t /*unused*/)
{
	return std::string(s);
}