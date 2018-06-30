#pragma once


template <class R>
class Visitor
{
public:
	virtual R visit(class Function& function) = 0;
	virtual R visit(class GraphicalFunction& graphicalFunction) = 0;
	virtual R visit(class FunctionBlock& functionBlock) = 0;
	virtual R visit(class Connection& connection) = 0;
	virtual R visit(class ValueBlock& valueBlock) = 0;
};

#define MAKE_VISITABLE_TYPED(R) virtual R accept(Visitor<R>& visitor) {return visitor.visit(*this);}
#define MAKE_VISITABLE MAKE_VISITABLE_TYPED(bool)

#define MAKE_VISITABLE_VIRTUAL_TYPED(R) virtual R accept(Visitor<R>& visitory) = 0;
#define MAKE_VISITABLE_VIRTUAL MAKE_VISITABLE_VIRTUAL_TYPED(bool)