#pragma once
#include <vector>
#include <programGraph/Block.h>
#include <programGraph/Datatype.h>
#include <programGraph/Connection.h>
#include <visitors/Visitor.h>
#include <memory>

class Function;

class FunctionBlock : public Block
{
public:
	typedef std::shared_ptr<FunctionBlock> Ptr;

	FunctionBlock(Function& function);
	virtual ~FunctionBlock();

	virtual std::vector<Datatype> inputTypes() const override;
	virtual std::vector<Datatype> outputTypes() const override;

	Function& function() const;
	
	MAKE_VISITABLE;

protected:
	Function& m_function;
private:
};