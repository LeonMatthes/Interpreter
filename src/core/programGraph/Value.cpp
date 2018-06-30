#include <programGraph/Value.h>
#include <error/InternalError.h>

Value::Value(bool b)
	: m_type(Datatype::BOOLEAN)
{
	m_data.m_boolean = b;
}

Value::Value(double d)
	: m_type(Datatype::DOUBLE)
{
	m_data.m_double = d;
}

void Value::assertType(Datatype type, std::string expectedType)
{
	if (m_type != type)
	{
		THROW_ERROR(InternalError, std::string("Type mismatch: trying to access value with type ") + expectedType + ", but actual type is different");
	}
}

bool Value::getBoolean()
{
	assertType(Datatype::BOOLEAN, "double");

	return m_data.m_boolean;
}

double Value::getDouble()
{
	assertType(Datatype::DOUBLE, "double");
	return m_data.m_double;
}

