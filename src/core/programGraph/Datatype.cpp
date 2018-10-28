#include <programGraph/Datatype.h>
#include <error/InternalError.h>

std::string operator+(std::string str, Datatype type)
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
