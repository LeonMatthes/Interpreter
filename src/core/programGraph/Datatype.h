#pragma once
#include <string>

enum class Datatype {
	DOUBLE,
	BOOLEAN
};

std::string operator+(std::string str, Datatype type);