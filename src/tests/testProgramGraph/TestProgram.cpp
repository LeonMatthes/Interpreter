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
	auto graphical = GraphicalFunction::UPtr(new GraphicalFunction());
	ASSERT_NO_THROW(Program(std::move(graphical), {}));
}

#include <programGraph/ReturnBlock.h>

TEST(TestProgram, TestRunFalseReturn)
{
	auto returnType = Datatype::BOOLEAN;
	auto graphical = GraphicalFunction::UPtr(new GraphicalFunction({}, { returnType }));
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
	auto graphical = GraphicalFunction::UPtr(new GraphicalFunction({}, { returnType }));
	auto returnStmt = std::make_shared<ReturnBlock>(*graphical);
	auto returnValue = Value(true);
	auto valueBlock = std::make_shared<ValueBlock>(returnValue);
	returnStmt->setInputConnections(std::vector<Connection>({ Connection(valueBlock, 0) }));
	graphical->setStatementBlocks({ returnStmt });
	graphical->setExpressionBlocks({ valueBlock });

	auto program = Program(std::move(graphical), {});
	ASSERT_EQ(std::vector<Value>({ returnValue }), program.run({}));
}