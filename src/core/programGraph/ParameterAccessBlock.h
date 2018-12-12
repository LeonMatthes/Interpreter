#pragma once
#include <programGraph/ExpressionBlock.h>
#include <programGraph/GraphicalFunction.h>

class ParameterAccessBlock : public ExpressionBlock
{
public:
	explicit ParameterAccessBlock(const GraphicalFunction& graphical);
	~ParameterAccessBlock() override = default;

	std::vector<Datatype> inputTypes() const override;
	std::vector<Datatype> outputTypes() const override;

	OVERRIDE_VISITABLE;

protected:
	const GraphicalFunction& m_function;
private:
};