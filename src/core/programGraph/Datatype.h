#pragma once
#include <string>

enum class Datatype {
	DOUBLE,
	BOOLEAN
};

std::string operator+(const std::string& str, Datatype type);