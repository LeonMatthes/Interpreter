#pragma once
#include <programGraph/ExpressionBlock.h>
#include <programGraph/GraphicalFunction.h>

class ParameterAccessBlock : public ExpressionBlock
{
public:
	ParameterAccessBlock(const GraphicalFunction& function);
	virtual ~ParameterAccessBlock() = default;

	virtual std::vector<Datatype> inputTypes() const override;
	virtual std::vector<Datatype> outputTypes() const override;

	MAKE_VISITABLE;

protected:
	const GraphicalFunction& m_function;
private:
};