#include <programGraph/Datatype.h>
#include <error/InternalError.h>

std::string operator+(std::string str, Datatype type)
{
	switch (type)
	{
	case Datatype::DOUBLE:
		return "double";
		break;
	case Datatype::BOOLEAN:
		return "boolean";
		break;
	}
	THROW_ERROR(InternalError, "Trying to convert Datatype to string, which is not supported");
}
