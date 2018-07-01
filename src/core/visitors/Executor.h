#pragma once

#include <visitors/Visitor.h>
#include <programGraph/Value.h>
#include <vector>
#include <stack>

class Executor : public Visitor<std::vector<Value>>
{
public:
	Executor();
	virtual ~Executor() = default;

	virtual std::vector<Value> visit(class Function& function) override;
	virtual std::vector<Value> visit(class GraphicalFunction& graphicalFunction) override;
	virtual std::vector<Value> visit(class FunctionBlock& functionBlock) override;
	virtual std::vector<Value> visit(class Connection& connection) override;
	virtual std::vector<Value> visit(class ValueBlock& valueBlock) override;
	virtual std::vector<Value> visit(class PrimitiveFunction& primitiveFunction) override;

	void pushParameters(std::vector<Value> parameters);
protected:
	std::stack<std::vector<Value>> m_callStack;

private:
};