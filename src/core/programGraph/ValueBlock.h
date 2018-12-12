#pragma once

#include <programGraph/ExpressionBlock.h>
#include <programGraph/Value.h>

class ValueBlock : public ExpressionBlock
{
public:
	ValueBlock() = delete;
	explicit ValueBlock(const Value& value);

	~ValueBlock() override;

	std::vector<Datatype> inputTypes() const override;
	std::vector<Datatype> outputTypes() const override;

	OVERRIDE_VISITABLE;

	Value value() const;
protected:
	Value m_value;

private:

};
