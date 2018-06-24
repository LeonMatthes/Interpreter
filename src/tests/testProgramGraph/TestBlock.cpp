#include "gtest/gtest.h"
#include <programGraph/FunctionBlock.h>
#include <programGraph/Function.h>
#include <error/Error.h>


class MockBlock : public Block {
public:
	MockBlock() : Block(inputTypes().size()) {}
	virtual std::vector<Datatype> inputTypes() const override
	{
		return { Datatype::DOUBLE, Datatype::BOOLEAN };
	}

	virtual std::vector<Datatype> outputTypes() const override
	{
		return { Datatype::BOOLEAN, Datatype::DOUBLE };
	}
};

TEST(TestBlock, creationSetsConnectionCount)
{
	MockBlock mockBlock;
	EXPECT_EQ(mockBlock.inputCount(), mockBlock.inputConnections().size());
}

TEST(TestBlock, setConnectionChecksSize)
{
	MockBlock mockBlock;
	std::vector<Connection> correctConnections(2);
	EXPECT_NO_THROW(mockBlock.setInputConnections(correctConnections));
	
	std::vector<Connection> tooFewConnections(0);
	EXPECT_THROW(mockBlock.setInputConnections(tooFewConnections), Error::Ptr);
	
	std::vector<Connection> tooManyConnections(3);
	EXPECT_THROW(mockBlock.setInputConnections(tooManyConnections), Error::Ptr);
}


TEST(TestFunctionBlock, creationSetsConnectionCount)
{
	Function function({ Datatype::BOOLEAN }, {});
	FunctionBlock block(function);
	ASSERT_EQ(1, block.inputConnections().size());
}

TEST(TestFunctionBlock, creationSetsFunction)
{
	Function function;
	FunctionBlock block(function);
	EXPECT_EQ(&block.function(), &function);
}

TEST(TestFunctionBlock, IOTypesMatchFunction)
{
	Function function({ Datatype::DOUBLE }, { Datatype::DOUBLE, Datatype::DOUBLE });
	FunctionBlock block(function);
	EXPECT_EQ(block.inputTypes(), function.inputs());
	EXPECT_EQ(block.outputTypes(), function.outputs());
}