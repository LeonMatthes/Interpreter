#pragma once

#include <programGraph/ExpressionBlock.h>
#include <programGraph/GraphicalFunction.h>

class VariableReadBlock : public ExpressionBlock
{
public:
	VariableReadBlock(const GraphicalFunction& owningFunction, VariableIdentifier identifier);
	~VariableReadBlock() override;

	std::vector<Datatype> inputTypes() const override;
	std::vector<Datatype> outputTypes() const override;

	const VariableIdentifier& variableIdentifier() const;

	OVERRIDE_VISITABLE;

protected:
	const GraphicalFunction& m_owningFunction;
	VariableIdentifier m_identifier;
private:
};