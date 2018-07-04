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

	bool checkOutputConnections(class GraphicalFunction &graphicalFunction);
protected:
	
private:
};