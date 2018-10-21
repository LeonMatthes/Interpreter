#pragma once

#include <visitors/Visitor.h>

class TypeChecker : public Visitor<bool>
{
public:
	TypeChecker();
	virtual ~TypeChecker();

	virtual bool visit(class GraphicalFunction& graphicalFunction) override;
	virtual bool visit(class FunctionBlock& functionBlock) override;
	virtual bool visit(class Connection& connection) override;
	virtual bool visit(class ValueBlock& valueBlock) override;
	virtual bool visit(class PrimitiveFunction& primitiveFunction) override;
	virtual bool visit(class VariableReadBlock& variableReadBlock) override;
	virtual bool visit(class ParameterAccessBlock& parameterAccess) override;

	virtual bool visit(class ReturnBlock& returnBlock) override;
	virtual bool visit(class ExpressionStatement& expressionStatement) override;
	virtual bool visit(class VariableWriteBlock& variableWriteBlock) override;
	virtual bool visit(class IfStatement& ifStatement) override;
	virtual bool visit(class WhileStatement& whileStatement) override;
protected:
	
	bool checkInputTypes(class Block& block);
private:
};