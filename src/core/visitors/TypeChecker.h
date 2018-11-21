#pragma once

#include <visitors/Visitor.h>

class TypeChecker : public Visitor<bool>
{
	using VisitorType = bool;

public:
	TypeChecker();
	virtual ~TypeChecker();

	virtual VisitorType visit(class GraphicalFunction& graphicalFunction) override;
	virtual VisitorType visit(class FunctionBlock& functionBlock) override;
	virtual VisitorType visit(class Connection& connection) override;
	virtual VisitorType visit(class ValueBlock& valueBlock) override;
	virtual VisitorType visit(class PrimitiveFunction& primitiveFunction) override;
	virtual VisitorType visit(class VariableReadBlock& variableReadBlock) override;
	virtual VisitorType visit(class ParameterAccessBlock& parameterAccess) override;

	virtual VisitorType visit(class ReturnBlock& returnBlock) override;
	virtual VisitorType visit(class ExpressionStatement& expressionStatement) override;
	virtual VisitorType visit(class VariableWriteBlock& variableWriteBlock) override;
	virtual VisitorType visit(class IfStatement& ifStatement) override;
	virtual VisitorType visit(class WhileStatement& whileStatement) override;
protected:
	
	VisitorType checkInputTypes(class Block& block);

	GraphicalFunction* m_currentFunction;
private:
};