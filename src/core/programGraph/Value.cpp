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

Value::~Value()
{

}

void Value::assertType(Datatype type)
{
	if (m_type != type)
	{
		THROW_ERROR(InternalError, std::string("Type mismatch: trying to access value with type ") + type + ", but actual type is: " + m_type);
	}
}

bool Value::getBoolean()
{
	assertType(Datatype::BOOLEAN);

	return m_data.m_boolean;
}

double Value::getDouble()
{
	assertType(Datatype::DOUBLE);
	return m_data.m_double;
}

Datatype Value::type() const
{
	return m_type;
}

