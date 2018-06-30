#pragma once
#include <programGraph/Datatype.h>
#include <string>

class Value
{
public:
	Value() = delete;
	Value(bool b);
	Value(double d);
	virtual ~Value() = default;

	bool getBoolean();

	void assertType(Datatype type, std::string expectedType);

	double getDouble();

protected:
	Datatype m_type;
	union {
		double m_double;
		bool m_boolean;
	} m_data;
private:
};