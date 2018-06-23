#include "gtest/gtest.h"
#include <programGraph/Block.h>
#include <programGraph/Function.h>

TEST(TestBlock, blockCreationSetsFunction)
{
	//Put in block to test destructor
	{
		Function function;
		Block block(function);
		EXPECT_EQ(&block.function(), &function);
	}
}

TEST(TestBlock, IOTypesMatchFunction)
{
	Function function({ Datatype::DOUBLE }, { Datatype::DOUBLE, Datatype::DOUBLE });
	Block block(function);
	EXPECT_EQ(block.inputTypes(), function.inputs());
	EXPECT_EQ(block.outputTypes(), function.outputs());
}