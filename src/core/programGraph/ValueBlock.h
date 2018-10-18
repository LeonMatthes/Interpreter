#pragma once

#include <programGraph/ExpressionBlock.h>
#include <programGraph/Value.h>

class ValueBlock : public ExpressionBlock
{
public:
	ValueBlock() = delete;
	ValueBlock(Value value);

	~ValueBlock();

	virtual std::vector<Datatype> inputTypes() const override;
	virtual std::vector<Datatype> outputTypes() const override;

	MAKE_VISITABLE;

	Value value() const;
	//do not use during Runtime!
	void value(Value val); 
protected:
	Value m_value;

private:

};
