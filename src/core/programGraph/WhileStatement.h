#pragma once

#include <programGraph/StatementBlock.h>

class WhileStatement : public StatementBlock
{
public:
	WhileStatement();
	virtual ~WhileStatement() = default;

	virtual std::vector<Datatype> inputTypes() const override;
	virtual std::vector<Datatype> outputTypes() const override;

	OVERRIDE_VISITABLE;

protected:
	
private:
};