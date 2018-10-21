#pragma once
#include <stdexcept>

template <class R>
class Visitor
{
public:
	//ExpressionBlocks
	virtual R visit(class GraphicalFunction& graphicalFunction) = 0;
	virtual R visit(class FunctionBlock& functionBlock) = 0;
	virtual R visit(class Connection& connection) = 0;
	virtual R visit(class ValueBlock& valueBlock) = 0;
	virtual R visit(class PrimitiveFunction& primitiveFunction) = 0;
	virtual R visit(class VariableReadBlock& variableReadBlock) = 0;
	virtual R visit(class ParameterAccessBlock& parameterAccess) = 0;

	//StatementBlocks
	virtual R visit(class ReturnBlock& returnBlock) = 0;
	virtual R visit(class ExpressionStatement& expressionStatement) = 0;
	virtual R visit(class VariableWriteBlock& variableWriteBlock) = 0;
	virtual R visit(class IfStatement& ifStatement) = 0;
	virtual R visit(class WhileStatement& whileStatement) = 0;

};

#include <vector>
#include <programGraph/Value.h>
#define MAKE_VISITABLE_TYPED(R) virtual R accept(Visitor<R>& visitor) {return visitor.visit(*this);}
#define MAKE_VISITABLE \
	MAKE_VISITABLE_TYPED(bool)\
	MAKE_VISITABLE_TYPED(std::vector<Value>)\
	MAKE_VISITABLE_TYPED(void)

#define MAKE_VISITABLE_VIRTUAL_TYPED(R) virtual R accept(Visitor<R>& visitory) = 0;
#define MAKE_VISITABLE_VIRTUAL \
	MAKE_VISITABLE_VIRTUAL_TYPED(bool)\
	MAKE_VISITABLE_VIRTUAL_TYPED(std::vector<Value>)\
	MAKE_VISITABLE_VIRTUAL_TYPED(void)

//For testing purposes
#define MOCK_VISITABLE_TYPED(R) virtual R accept(Visitor<R>& visitor) { throw std::logic_error("Error: Function not implemented"); }
#define MOCK_VISITABLE \
	MOCK_VISITABLE_TYPED(bool)\
	MOCK_VISITABLE_TYPED(std::vector<Value>)\
	MOCK_VISITABLE_TYPED(void)
