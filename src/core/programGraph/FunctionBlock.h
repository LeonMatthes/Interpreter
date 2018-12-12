#pragma once
#include <memory>
#include <programGraph/Connection.h>
#include <programGraph/Datatype.h>
#include <programGraph/ExpressionBlock.h>
#include <vector>
#include <visitors/Visitor.h>

class Function;

class FunctionBlock : public ExpressionBlock
{
public:
	using Ptr = std::shared_ptr<FunctionBlock>;

	explicit FunctionBlock(Function& function);
	~FunctionBlock() override;

	std::vector<Datatype> inputTypes() const override;
	std::vector<Datatype> outputTypes() const override;

	Function& function() const;
	
	OVERRIDE_VISITABLE;
protected:
	Function& m_function;
private:
};