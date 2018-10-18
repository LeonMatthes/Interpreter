#include "gtest/gtest.h"
#include <programGraph/FunctionBlock.h>
#include <testProgramGraph/MockFunction.h>
#include <programGraph/ValueBlock.h>
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

	MOCK_VISITABLE;
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
	MockFunction function({ Datatype::BOOLEAN }, {});
	FunctionBlock block(function);
	ASSERT_EQ(1, block.inputConnections().size());
}

TEST(TestFunctionBlock, creationSetsFunction)
{
	MockFunction function;
	FunctionBlock block(function);
	EXPECT_EQ(&block.function(), &function);
}

TEST(TestFunctionBlock, IOTypesMatchFunction)
{
	MockFunction function({ Datatype::DOUBLE }, { Datatype::DOUBLE, Datatype::DOUBLE });
	FunctionBlock block(function);
	EXPECT_EQ(block.inputTypes(), function.inputs());
	EXPECT_EQ(block.outputTypes(), function.outputs());
}



TEST(TestValueBlock, creation)
{
	ValueBlock block(Value(false));
	EXPECT_EQ(0, block.inputCount());
	EXPECT_EQ(std::vector<Datatype>({ Datatype::BOOLEAN }), block.outputTypes());
	EXPECT_EQ(std::vector<Datatype>({}), block.inputTypes());

	block = ValueBlock(Value(1.0));
	EXPECT_EQ(std::vector<Datatype>({ Datatype::DOUBLE }), block.outputTypes());
}


#include <programGraph/WhileStatement.h>
TEST(TestWhileBlock, IOTypes)
{
	WhileStatement whileStatment;
	EXPECT_EQ(whileStatment.outputCount(), 0);
	EXPECT_EQ(whileStatment.inputTypes(), std::vector<Datatype>({ Datatype::BOOLEAN }));
	EXPECT_EQ(whileStatment.flowConnectionsCount(), 2);
}