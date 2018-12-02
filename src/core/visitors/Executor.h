#pragma once

#include <visitors/Visitor.h>
#include <visitors/Evaluator.h>
#include <vector>
#include <programGraph/Value.h>
#include <programGraph/GraphicalFunction.h>
#include <unordered_map>
#include <map>
#include <stack>

struct Return
{
	std::vector<Value> m_values;
};

struct StackFrame
{
	std::map<VariableIdentifier, Value> m_variables;
	std::vector<Value> m_parameters;
};

class Executor : public Visitor<void>
{
public:
	Executor();
	virtual ~Executor();

	void visit(class GraphicalFunction& graphicalFunction) override;
	void visit(class FunctionBlock& functionBlock) override;
	void visit(class Connection& connection) override;
	void visit(class ValueBlock& valueBlock) override;
	void visit(class PrimitiveFunction& primitiveFunction) override;
	void visit(class VariableReadBlock& variableReadBlock) override;
	void visit(class ParameterAccessBlock& parameterAccess) override;

	void visit(class ReturnBlock& returnBlock) override;
	void visit(class ExpressionStatement& expressionStatement) override;
	void visit(class VariableWriteBlock& variableWriteBlock) override;
	void visit(class IfStatement& ifStatement) override;
	void visit(class WhileStatement& whileStatement) override;
	
	bool executeNext(class StatementBlock& statement);
	bool executeNext(class StatementBlock& statement, size_t flowConnectionIndex);

	std::vector<Value> evaluate(class StatementBlock& statement);
	std::vector<Value> evaluate(class GraphicalFunction& graphicalFunction);
	std::vector<Value> evaluate(class GraphicalFunction& graphicalFunction, std::vector<Value> parameters);
	Value variableValue(VariableIdentifier identifier);
	std::vector<Value> parameters() const;
protected:
	Evaluator m_evaluator;
	std::unordered_map<class StatementBlock*, std::vector<Value>> m_executedStatements;
	std::stack<StackFrame, std::vector<StackFrame>> m_callStack;

	void throwExpressionError();
private:
};