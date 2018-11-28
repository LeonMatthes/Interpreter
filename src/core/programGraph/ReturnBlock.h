#pragma once

#include <programGraph/StatementBlock.h>

class ReturnBlock : public StatementBlock
{
public:
	ReturnBlock() = delete;
	ReturnBlock(class Function& function);
	virtual ~ReturnBlock();

	virtual std::vector<Datatype> inputTypes() const override;
	virtual std::vector<Datatype> outputTypes() const override;

	OVERRIDE_VISITABLE;

protected:
	Function& m_function;
private:
};