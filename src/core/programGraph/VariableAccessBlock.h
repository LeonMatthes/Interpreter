#pragma once

#include <programGraph/ExpressionBlock.h>
#include <programGraph/GraphicalFunction.h>

class VariableAccessBlock : public ExpressionBlock
{
public:
	VariableAccessBlock(const GraphicalFunction& owningFunction, VariableIdentifier identifier);
	virtual ~VariableAccessBlock();

	virtual std::vector<Datatype> inputTypes() const override;
	virtual std::vector<Datatype> outputTypes() const override;

	MOCK_VISITABLE;

protected:
	const GraphicalFunction& m_owningFunction;
	VariableIdentifier m_identifier;
private:
};