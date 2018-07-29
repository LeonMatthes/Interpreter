#pragma once

#include <visitors/Visitor.h>
#include <visitors/Evaluator.h>
#include <vector>
#include <programGraph/Value.h>
#include <unordered_map>

struct Return
{
	std::vector<Value> m_values;
};

class Executor : public Visitor<void>
{
public:
	Executor();
	virtual ~Executor();

	virtual void visit(class GraphicalFunction& graphicalFunction) override;
	virtual void visit(class FunctionBlock& functionBlock) override;
	virtual void visit(class Connection& connection) override;
	virtual void visit(class ValueBlock& valueBlock) override;
	virtual void visit(class PrimitiveFunction& primitiveFunction) override;
	virtual void visit(class ReturnBlock& returnBlock) override;
	virtual void visit(class ExpressionStatement& expressionStatement) override;
	
	std::vector<Value> evaluate(class StatementBlock& statement);
	std::vector<Value> evaluate(class GraphicalFunction& graphicalFunction);
protected:
	Evaluator m_evaluator;
	std::unordered_map<class StatementBlock*, std::vector<Value>> m_executedStatements;

	void throwExpressionError();
private:
};