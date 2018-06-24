#include <gtest/gtest.h>
#include <programGraph/Function.h>
#include <programGraph/GraphicalFunction.h>
#include <programGraph/FunctionBlock.h>
#include <memory>

TEST(TestFunction, createEmptyFunction)
{
	Function function;
	EXPECT_EQ(function.inputs().size(), 0);
	EXPECT_EQ(function.outputs().size(), 0);
}

TEST(TestFunction, createFunctionWithIO)
{
	std::vector<Datatype> in = { Datatype::DOUBLE };
	std::vector<Datatype> out = { Datatype::DOUBLE,  Datatype::DOUBLE };
	Function function(in, out);
	EXPECT_EQ(function.inputs(), in);
	EXPECT_EQ(function.outputs(), out);
}

TEST(TestGraphicalFunction, testUniquePtrOwnership)
{
	Function function({ Datatype::DOUBLE }, {});
	GraphicalFunction graphical;
	
	{
		FunctionBlock::UPtr block = std::make_unique<FunctionBlock>(function);
		ASSERT_TRUE(block);
		std::vector<FunctionBlock::UPtr> blocks;
		blocks.push_back(std::move(block));
		ASSERT_FALSE(block);
		
		ASSERT_TRUE(blocks.at(0));
		graphical.setFunctionBlocks(std::move(blocks));
		ASSERT_EQ(0, blocks.size());

		ASSERT_TRUE(graphical.functionBlocks().at(0));
	}

}