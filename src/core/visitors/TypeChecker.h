#pragma once
#include <visitors/TypeCheckResult.h>
#include <visitors/Visitor.h>

class TypeChecker : public Visitor<TypeCheckResult>
{
	using VisitorType = TypeCheckResult;

public:
	TypeChecker();
	virtual ~TypeChecker();

	VisitorType visit(class GraphicalFunction& graphicalFunction) override;
	VisitorType visit(class FunctionBlock& functionBlock) override;
	VisitorType visit(class Connection& connection) override;
	VisitorType visit(class ValueBlock& valueBlock) override;
	VisitorType visit(class PrimitiveFunction& primitiveFunction) override;
	VisitorType visit(class VariableReadBlock& variableReadBlock) override;
	VisitorType visit(class ParameterAccessBlock& parameterAccess) override;

	VisitorType visit(class ReturnBlock& returnBlock) override;
	VisitorType visit(class ExpressionStatement& expressionStatement) override;
	VisitorType visit(class VariableWriteBlock& variableWriteBlock) override;
	VisitorType visit(class IfStatement& ifStatement) override;
	VisitorType visit(class WhileStatement& whileStatement) override;
protected:
	
	VisitorType checkInputTypes(class Block& block);

	GraphicalFunction* m_currentFunction{nullptr};
private:
};