#include <programGraph/Value.h>
#include <error/InternalError.h>

Value::Value()
	: Value(false)
{

}

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

Value::Value(Datatype datatype)
	: m_type(datatype)
{
	switch (m_type)
	{
	case Datatype::DOUBLE:
		m_data.m_double = 0.0;
		break;
	case Datatype::BOOLEAN:
		m_data.m_boolean = false;
		break;
	default:
		THROW_ERROR(InternalError, std::string("Value constructed with type: ") + m_type + " does not have a default constructor");
		break;
	}
}

Value::~Value()
{

}

bool Value::operator==(const Value& RHS) const
{
	if (RHS.m_type != m_type)
	{
		return false;
	}

	switch (m_type)
	{
	case Datatype::DOUBLE:
		return getDouble() == RHS.getDouble();
		break;
	case Datatype::BOOLEAN:
		return getBoolean() == RHS.getBoolean();
		break;
	default:
		break;
	}
	THROW_ERROR(InternalError, std::string("Illegal program flow reached in function Value::operator=="));
}

void Value::assertType(Datatype type) const
{
	if (m_type != type)
	{
		THROW_ERROR(InternalError, std::string("Type mismatch: trying to access value with type ") + type + ", but actual type is: " + m_type);
	}
}

bool Value::getBoolean() const
{
	assertType(Datatype::BOOLEAN);

	return m_data.m_boolean;
}

double Value::getDouble() const
{
	assertType(Datatype::DOUBLE);
	return m_data.m_double;
}

Datatype Value::type() const
{
	return m_type;
}