#pragma once
#include <vector>
#include <programGraph/Block.h>
#include <programGraph/Datatype.h>
#include <programGraph/Connection.h>
#include <memory>

class Function;

class FunctionBlock : public Block
{
public:
	typedef std::unique_ptr<FunctionBlock> UPtr;

	FunctionBlock(Function& function);
	virtual ~FunctionBlock();

	virtual std::vector<Datatype> inputTypes() const override;
	virtual std::vector<Datatype> outputTypes() const override;

	Function& function() const;
protected:
	Function& m_function;
private:
};