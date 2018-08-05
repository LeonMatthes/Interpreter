#pragma once

#include <visitors/Visitor.h>
#include <programGraph/Value.h>
#include <vector>
#include <stack>

class Evaluator : public Visitor<std::vector<Value>>
{
public:
	Evaluator(class Executor& executor);
	virtual ~Evaluator();

	virtual std::vector<Value> visit(class GraphicalFunction& graphicalFunction) override;
	virtual std::vector<Value> visit(class FunctionBlock& functionBlock) override;
	virtual std::vector<Value> visit(class Connection& connection) override;
	virtual std::vector<Value> visit(class ValueBlock& valueBlock) override;
	virtual std::vector<Value> visit(class PrimitiveFunction& primitiveFunction) override;
	virtual std::vector<Value> visit(class ReturnBlock& ReturnBlock) override;
	virtual std::vector<Value> visit(class ExpressionStatement& expressionStatement) override;
	virtual std::vector<Value> visit(class VariableReadBlock& variableReadBlock) override;

	void pushParameters(std::vector<Value> parameters);


protected:
	std::stack<std::vector<Value>> m_callStack;

	class Executor& m_executor;
private:
};