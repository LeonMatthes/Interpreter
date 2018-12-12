#pragma once

#include <programGraph/Value.h>
#include <stack>
#include <vector>
#include <visitors/Visitor.h>

class Evaluator : public Visitor<std::vector<Value>>
{
public:
	explicit Evaluator(class Executor& executor);
	virtual ~Evaluator();

	std::vector<Value> visit(class GraphicalFunction& graphicalFunction) override;
	std::vector<Value> visit(class FunctionBlock& functionBlock) override;
	std::vector<Value> visit(class Connection& connection) override;
	std::vector<Value> visit(class ValueBlock& valueBlock) override;
	std::vector<Value> visit(class PrimitiveFunction& primitiveFunction) override;
	std::vector<Value> visit(class ParameterAccessBlock& parameterAccess) override;


	std::vector<Value> visit(class ReturnBlock& ReturnBlock) override;
	std::vector<Value> visit(class ExpressionStatement& expressionStatement) override;
	std::vector<Value> visit(class VariableReadBlock& variableReadBlock) override;
	std::vector<Value> visit(class VariableWriteBlock& variableWriteBlock) override;
	std::vector<Value> visit(class IfStatement& ifStatement) override;
	std::vector<Value> visit(class WhileStatement& whileStatement) override;

	void pushParameters(const std::vector<Value>& parameters);
	Value evaluateConnection(class Connection& connection, Datatype type);


protected:
	std::stack<std::vector<Value>> m_callStack;

	class Executor& m_executor;
private:
};