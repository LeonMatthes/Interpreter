#pragma once
#include <programGraph/Datatype.h>
#include <string>

class Value
{
public:
	Value() = delete;
	Value(bool b);
	Value(double d);
	virtual ~Value();

	bool getBoolean() const;
	double getDouble() const;
	Datatype type() const;

	void assertType(Datatype type) const;

	bool operator==(const Value& RHS) const;
protected:
	Datatype m_type;
	union {
		double m_double;
		bool m_boolean;
	} m_data;
private:
};