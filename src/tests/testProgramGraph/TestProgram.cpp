#include <gtest/gtest.h>

#include <programGraph/Program.h>
#include <programGraph/GraphicalFunction.h>
#include <testUtility/CustomAssert.h>
#include <error/InternalError.h>

TEST(TestProgram, TestEmptyCreation)
{
	ASSERT_THROW_INTERPRETER(Program(nullptr, {}), InternalError);
}

TEST(TestProgram, TestOnlyStartFunction)
{
    auto graphical = std::make_unique<GraphicalFunction>();
    ASSERT_NO_THROW(Program(std::move(graphical), {}));
}

#include <programGraph/ReturnBlock.h>

TEST(TestProgram, TestRunFalseReturn)
{
	auto returnType = Datatype::BOOLEAN;
	auto graphical = std::make_unique<GraphicalFunction>(std::vector<Datatype>(), std::vector<Datatype>({ returnType }));
	auto returnStmt = std::make_shared<ReturnBlock>(*graphical);
	graphical->setStatementBlocks({ returnStmt });

	auto program = Program(std::move(graphical), {});
	ASSERT_EQ(std::vector<Value>({returnType}), program.run({}));
}

#include <programGraph/Connection.h>
#include <programGraph/ValueBlock.h>

TEST(TestProgram, TestRunTrueReturn)
{
	auto returnType = Datatype::BOOLEAN;
	auto graphical = std::make_unique<GraphicalFunction>(std::vector<Datatype>(), std::vector<Datatype>({ returnType }));
	auto returnStmt = std::make_shared<ReturnBlock>(*graphical);
	auto returnValue = Value(true);
	auto valueBlock = std::make_shared<ValueBlock>(returnValue);
	returnStmt->setInputConnections(std::vector<Connection>({ Connection(valueBlock, 0) }));
	graphical->setStatementBlocks({ returnStmt });
	graphical->setExpressionBlocks({ valueBlock });

	auto program = Program(std::move(graphical), {});
	ASSERT_EQ(std::vector<Value>({ returnValue }), program.run({}));
}

#include <programGraph/ParameterAccessBlock.h>

#include <memory>
TEST(TestProgram, TestRunParameters)
{
	auto input1 = Datatype::BOOLEAN;
	auto input2 = Datatype::DOUBLE;
	auto graphical = std::make_unique<GraphicalFunction>(std::vector<Datatype>({ input1, input2 }), std::vector<Datatype>({ input1, input2 }));
	auto returnStatement = std::make_shared<ReturnBlock>(*graphical);
	auto parameterAccess = std::make_shared<ParameterAccessBlock>(*graphical);
	returnStatement->setInputConnections({ Connection(parameterAccess, 0), Connection(parameterAccess, 1) });
	graphical->setStatementBlocks({ returnStatement });
	graphical->setExpressionBlocks({ parameterAccess });

	auto program = Program(std::move(graphical), {});

	auto inputValue1 = Value(false);
	auto inputValue2 = Value(2.0);
	EXPECT_EQ(std::vector<Value>({ inputValue1, inputValue2 }), program.run({ inputValue1, inputValue2 }));
}