#pragma once
#include <programGraph/Datatype.h>
#include <string>


class Value
{
public:
	Value();
	virtual ~Value();

	bool getBoolean() const;
	double getDouble() const;
	Datatype type() const;

	void assertType(Datatype type) const;

	// These implicit conversions are allowed for readability
	Value(bool b); // NOLINT(google-explicit-constructor)
	Value(double d); // NOLINT(google-explicit-constructor)
	Value(Datatype datatype); // NOLINT(google-explicit-constructor)
	bool operator==(const Value& RHS) const; // NOLINT(google-explicit-constructor)
protected:
	Datatype m_type;

	union Data {
		double m_double;
		bool m_boolean;
	};
	Value::Data m_data{ 0.0 };
private:
};