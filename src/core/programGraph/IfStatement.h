#pragma once

#include <programGraph/StatementBlock.h>

class IfStatement : public StatementBlock
{
public:
	IfStatement();
	virtual ~IfStatement();

	virtual std::vector<Datatype> inputTypes() const override;
	virtual std::vector<Datatype> outputTypes() const override;

	OVERRIDE_VISITABLE;

protected:
	
private:
};