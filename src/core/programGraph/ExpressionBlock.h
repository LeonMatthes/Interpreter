#pragma once

#include <programGraph/Block.h>
#include <visitors/Visitor.h>

//this class exists to differentiate Expressions from Statements
//useful to decide between Functions and Methods
class ExpressionBlock : public Block
{
public:
	typedef std::shared_ptr<ExpressionBlock> Ptr;

	ExpressionBlock(size_t inputCount)
		: Block(inputCount)
	{}
	
	virtual ~ExpressionBlock() = default;

	MAKE_VISITABLE;
protected:
	
private:
};