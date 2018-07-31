#include <gtest/gtest.h>
#include <testProgramGraph/MockFunction.h>
#include <programGraph/GraphicalFunction.h>
#include <programGraph/FunctionBlock.h>
#include <testUtility/CustomAssert.h>
#include <error/InternalError.h>
#include <memory>

TEST(TestFunction, createEmptyFunction)
{
	MockFunction function;
	EXPECT_EQ(function.inputs().size(), 0);
	EXPECT_EQ(function.outputs().size(), 0);
}

TEST(TestFunction, createFunctionWithIO)
{
	std::vector<Datatype> in = { Datatype::DOUBLE };
	std::vector<Datatype> out = { Datatype::DOUBLE,  Datatype::DOUBLE };
	MockFunction function(in, out);
	EXPECT_EQ(function.inputs(), in);
	EXPECT_EQ(function.outputs(), out);
}

TEST(TestGraphicalFunction, createFunctionWithIO)
{
	std::vector<Datatype> in = { Datatype::DOUBLE };
	std::vector<Datatype> out = { Datatype::BOOLEAN, Datatype::DOUBLE };
	GraphicalFunction function(in, out);
	EXPECT_EQ(function.inputs(), in);
	EXPECT_EQ(function.outputs(), out);
}

TEST(TestGraphicalFunction, Variables)
{
	auto graphical = GraphicalFunction({}, {});
	auto variableName = "MyVariable";
	auto type = Datatype::DOUBLE;

	ASSERT_THROW_INTERPRETER(graphical.variableType(variableName), InternalError);
	graphical.addVariable(variableName, type);
	ASSERT_TRUE(graphical.hasVariable(variableName));
	ASSERT_EQ(graphical.variableType(variableName), type);
}