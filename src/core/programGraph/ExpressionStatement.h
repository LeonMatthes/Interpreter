#pragma once
#include <programGraph/StatementBlock.h>
#include <memory>

class ExpressionStatement : public StatementBlock
{
public:
	ExpressionStatement(std::unique_ptr<class ExpressionBlock> expression);
	ExpressionStatement(const ExpressionStatement&) = delete;
	virtual ~ExpressionStatement();

	virtual std::vector<Datatype> inputTypes() const override;
	virtual std::vector<Datatype> outputTypes() const override;

	virtual void setInputConnections(std::vector<Connection> val) override;

	MAKE_VISITABLE;
protected:
	
	std::unique_ptr<ExpressionBlock> m_expression;
private:
};