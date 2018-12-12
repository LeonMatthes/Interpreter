#include <error/InternalError.h>
#include <programGraph/Datatype.h>

std::string operator+(const std::string& str, Datatype type)
{
	switch (type)
	{
	case Datatype::DOUBLE:
		return str + "Double";
		break;
	case Datatype::BOOLEAN:
		return str + "Boolean";
		break;
	}
	THROW_ERROR(InternalError, "Trying to convert unsupported Datatype to string!");
}
