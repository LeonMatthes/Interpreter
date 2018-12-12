#include <gtest/gtest.h>
#include <visitors/Executor.h>
#include <testProgramGraph/MockFunction.h>
#include <programGraph/ReturnBlock.h>
#include <programGraph/ValueBlock.h>
#include <error/InternalError.h>
#include <error/RuntimeError.h>
#include <testUtility/CustomAssert.h>


class TestExecutor : public ::testing::Test
{
protected:
	Executor m_executor;
};

TEST_F(TestExecutor, EmptyReturnBlock)
{
	MockFunction emptyFunction({}, {});
	ReturnBlock block(emptyFunction);
	ASSERT_THROW(block.accept(m_executor), Return);
	try
	{
		block.accept(m_executor);
	}
	catch (Return& returnValues)
	{
		EXPECT_TRUE(returnValues.m_values.empty());
	}
}

TEST_F(TestExecutor, UnconnectedReturnBlock)
{
	MockFunction myFunction({}, { Datatype::DOUBLE });
	ReturnBlock block(myFunction);
	ASSERT_THROW(block.accept(m_executor), Return);
	try
	{
		block.accept(m_executor);
	}
	catch (Return& returnValues)
	{
		EXPECT_EQ(std::vector<Value>({ Value(Datatype::DOUBLE) }), returnValues.m_values);
	}
}

TEST_F(TestExecutor, ConnectedReturnBlock)
{
	Value returnedValue(2.0);

	MockFunction myFunction({}, { Datatype::DOUBLE });
	ValueBlock::Ptr valueBlock = std::make_shared<ValueBlock>(returnedValue);
	ReturnBlock returnBlock(myFunction);
	returnBlock.setInputConnections({ Connection(valueBlock, 0) });

	ASSERT_THROW(returnBlock.accept(m_executor), Return);
	try
	{
		returnBlock.accept(m_executor);
	}
	catch (Return& returnValues)
	{
		ASSERT_EQ(std::vector<Value>({ returnedValue }), returnValues.m_values);
	}
}

#include <programGraph/FunctionBlock.h>
#include <programGraph/PrimitiveFunction.h>
#include <programGraph/ValueBlock.h>
#include <error/Error.h>
TEST_F(TestExecutor, ExpressionBlocksNotCorrect)
{
	auto emptyFunction = MockFunction({}, {});
	auto functionBlock = FunctionBlock(emptyFunction);
	ASSERT_THROW_INTERPRETER(functionBlock.accept(m_executor), InternalError);

	auto primitive = PrimitiveFunction::add;
	ASSERT_THROW_INTERPRETER(primitive.accept(m_executor), InternalError);

	auto connection = Connection();
	ASSERT_THROW_INTERPRETER(connection.accept(m_executor), InternalError);

	auto valueBlock = ValueBlock(false);
	ASSERT_THROW_INTERPRETER(valueBlock.accept(m_executor), InternalError);
}

#include <programGraph/GraphicalFunction.h>
TEST_F(TestExecutor, EmptyGraphicalFunction)
{
	//Empty Graphical Function, does NOT! have a return block
	GraphicalFunction graphicalFunction;
	ASSERT_EQ(std::vector<Value>(), m_executor.evaluate(graphicalFunction));
}

TEST_F(TestExecutor, EmptyGraphicalFunctionWithReturnTypes)
{
	auto graphicalFunction = GraphicalFunction({}, { Datatype::BOOLEAN });
	ASSERT_THROW_INTERPRETER(m_executor.evaluate(graphicalFunction), RuntimeError);
}

TEST_F(TestExecutor, SimpleGraphicalFunctionWithReturnTypes)
{
	auto returnType = Datatype::BOOLEAN;
	auto graphicalFunction = GraphicalFunction({}, { returnType });
	auto returnStatement = std::make_shared<ReturnBlock>(graphicalFunction);
	graphicalFunction.setStatementBlocks({ returnStatement });

	ASSERT_EQ(std::vector<Value>({ returnType }), m_executor.evaluate(graphicalFunction));

	auto value = Value(2.0);
	auto valueBlock = std::make_shared<ValueBlock>(value);
	graphicalFunction.setExpressionBlocks({ valueBlock });
	returnStatement->setInputConnections({ Connection(valueBlock, 0) });
	ASSERT_EQ(value, m_executor.evaluate(graphicalFunction).at(0));
}

#include <programGraph/ExpressionStatement.h>
TEST_F(TestExecutor, MultipleStatements)
{
	auto returnType = Datatype::DOUBLE;
	auto graphical = GraphicalFunction({}, { returnType });
	auto expressionBlock = std::unique_ptr<ExpressionBlock>(new ValueBlock(Value(0.0)));
	auto statement = std::make_shared<ExpressionStatement>(std::move(expressionBlock));
	graphical.setStatementBlocks({ statement });

	ASSERT_THROW_INTERPRETER(m_executor.evaluate(graphical), RuntimeError);

	auto returnBlock = std::make_shared<ReturnBlock>(graphical);
	statement->setFlowConnections({ProgramFlowConnection(returnBlock)});
	graphical.setStatementBlocks({ statement, returnBlock });
	ASSERT_EQ(std::vector<Value>({ returnType }), m_executor.evaluate(graphical));
}

TEST_F(TestExecutor, MultipleConnectedStatements)
{
	auto returnType = Datatype::DOUBLE;
	auto graphical = GraphicalFunction({}, { returnType });
	auto value = Value(20.0);
	auto expressionBlock = std::unique_ptr<ExpressionBlock>(new ValueBlock(value));
	auto expressionStatement = std::make_shared<ExpressionStatement>(std::move(expressionBlock));
	
	auto returnBlock = std::make_shared<ReturnBlock>(graphical);
	expressionStatement->setFlowConnections({ ProgramFlowConnection(returnBlock) });
	returnBlock->setInputConnections({ Connection(expressionStatement, 0) });
	graphical.setStatementBlocks({ expressionStatement, returnBlock });

	ASSERT_EQ(std::vector<Value>({ value }), m_executor.evaluate(graphical));
}

TEST_F(TestExecutor, ForwardDataConnection)
{
	auto functionBlock = std::unique_ptr<ExpressionBlock>(new FunctionBlock(PrimitiveFunction::add));
	auto firstStatement = std::make_shared<ExpressionStatement>(std::move(functionBlock));
	
	auto valueBlock = std::unique_ptr<ExpressionBlock>(new ValueBlock(2.0));
	auto secondStatement = std::make_shared<ExpressionStatement>(std::move(valueBlock));
	
	firstStatement->setInputConnections({ Connection(secondStatement, 0), Connection()});
	auto graphical = GraphicalFunction();
	graphical.setStatementBlocks({ firstStatement, secondStatement });

	//Throw error when forward-connecting data, as its pretty much always a bug!
	ASSERT_THROW_INTERPRETER(m_executor.evaluate(graphical), RuntimeError);
}

TEST_F(TestExecutor, GraphicalFunctionInExpressionStatement)
{
	auto insideGraphical = GraphicalFunction({}, { Datatype::DOUBLE, Datatype::DOUBLE });
	
	auto firstValue = std::make_shared<ValueBlock>(Value(2.0));
	auto secondValue = std::make_shared<ValueBlock>(Value(1.0));
	insideGraphical.setExpressionBlocks({ firstValue, secondValue});

	auto returnBlock = std::make_shared<ReturnBlock>(insideGraphical);
	returnBlock->setInputConnections({ Connection(firstValue, 0), Connection(secondValue, 0) });
	insideGraphical.setStatementBlocks({ returnBlock });

	auto outsideGraphical = GraphicalFunction({}, { Datatype::DOUBLE, Datatype::DOUBLE });
	auto statement = ExpressionStatement::make_shared<FunctionBlock>(insideGraphical);
	auto outsideReturn = std::make_shared<ReturnBlock>(outsideGraphical);
	outsideReturn->setInputConnections({ Connection(statement, 0), Connection(statement, 1) });
	statement->setFlowConnections({ ProgramFlowConnection(outsideReturn) });
	outsideGraphical.setStatementBlocks({ statement, outsideReturn });

	ASSERT_EQ(std::vector<Value>({ 2.0, 1.0 }), m_executor.evaluate(outsideGraphical));
}

#include <programGraph/VariableReadBlock.h>
TEST_F(TestExecutor, ReadUninitializedVariable)
{
	auto type = Datatype::DOUBLE;
	auto variableID = "a Variable";
	auto graphical = GraphicalFunction({}, {type});
	graphical.addVariable(variableID, type);

	auto variableRead = std::make_shared<VariableReadBlock>(graphical, variableID);
	auto returnBlock = std::make_shared<ReturnBlock>(graphical);
	returnBlock->setInputConnections({ Connection(variableRead, 0) });
	graphical.setStatementBlocks({ returnBlock });
	graphical.setExpressionBlocks({ variableRead });
	ASSERT_EQ(std::vector<Value>({ Value(type) }), m_executor.evaluate(graphical));
}

#include <programGraph/VariableWriteBlock.h>
TEST_F(TestExecutor, ReadWriteVariable)
{
	auto type = Datatype::DOUBLE;
	auto variableID = "a Variable";
	auto graphical = GraphicalFunction({}, { type });
	auto value = -1.0;
	graphical.addVariable(variableID, type);

	auto variableWrite = std::make_shared<VariableWriteBlock>(graphical, variableID);
	auto valueBlock = std::make_shared<ValueBlock>(value);
	variableWrite->setInputConnections({ Connection(valueBlock, 0) });

	auto variableRead = std::make_shared<VariableReadBlock>(graphical, variableID);
	auto returnBlock = std::make_shared<ReturnBlock>(graphical);
	returnBlock->setInputConnections({ Connection(variableRead, 0) });
	variableWrite->setFlowConnections({ ProgramFlowConnection(returnBlock) });

	graphical.setStatementBlocks({ variableWrite, returnBlock });
	graphical.setExpressionBlocks({ valueBlock, variableRead });
	ASSERT_EQ(std::vector<Value>({ Value(value) }), m_executor.evaluate(graphical));

	auto secondValue = -2.0;
	valueBlock = std::make_shared<ValueBlock>(secondValue);
	variableWrite->setInputConnections({ Connection(valueBlock, 0) });
	graphical.setExpressionBlocks({ valueBlock, variableRead });
	ASSERT_EQ(std::vector<Value>({ Value(secondValue) }), m_executor.evaluate(graphical));
}

TEST_F(TestExecutor, VariableWriteOutput)
{
	auto type = Datatype::DOUBLE;
	auto variableID = "a Variable";
	auto graphical = GraphicalFunction({}, { type });
	auto value = -1.0;
	graphical.addVariable(variableID, type);

	auto variableWrite = std::make_shared<VariableWriteBlock>(graphical, variableID);
	auto valueBlock = std::make_shared<ValueBlock>(value);
	variableWrite->setInputConnections({ Connection(valueBlock, 0) });

	auto returnBlock = std::make_shared<ReturnBlock>(graphical);
	returnBlock->setInputConnections({ Connection(variableWrite, 0) });
	variableWrite->setFlowConnections({ ProgramFlowConnection(returnBlock) });

	graphical.setExpressionBlocks({ valueBlock });
	graphical.setStatementBlocks({ variableWrite, returnBlock });
	ASSERT_EQ(std::vector<Value>({ Value(value) }), m_executor.evaluate(graphical));
}


#include <programGraph/IfStatement.h>
TEST_F(TestExecutor, IfStatement)
{
	auto ifStatement = std::make_shared<IfStatement>();
	auto ifInput = std::make_shared<ValueBlock>(true);
	ifStatement->setInputConnections({ Connection(ifInput, 0) });

	auto returnType = Datatype::DOUBLE;
	auto graphical = GraphicalFunction({}, {returnType});
	
	auto returnStatement = std::make_shared<ReturnBlock>(graphical);
	ifStatement->setFlowConnections({ ProgramFlowConnection(returnStatement), ProgramFlowConnection() });

	graphical.setStatementBlocks({ ifStatement, returnStatement });
	graphical.setExpressionBlocks({ ifInput });

	ASSERT_EQ(m_executor.evaluate(graphical).front(), Value(returnType));

	ifInput = std::make_shared<ValueBlock>(false);
	ifStatement->setInputConnections({ Connection(ifInput, 0) });
	graphical.setExpressionBlocks({ifInput});
	ASSERT_THROW_INTERPRETER(m_executor.evaluate(graphical), RuntimeError);
}

#include <programGraph/WhileStatement.h>
TEST_F(TestExecutor, WhileStatement)
{
	auto whileStatement = std::make_shared<WhileStatement>();
	auto whileInput = std::make_shared<ValueBlock>(false);
	whileStatement->setInputConnections({ Connection(whileInput, 0) });

	auto returnType = Datatype::DOUBLE;
	auto graphical = GraphicalFunction({}, { returnType });

	auto returnStatement = std::make_shared<ReturnBlock>(graphical);
	whileStatement->setFlowConnections({ ProgramFlowConnection(returnStatement), ProgramFlowConnection() });

	graphical.setStatementBlocks({ whileStatement, returnStatement });
	ASSERT_EQ(m_executor.evaluate(graphical).front(), Value(returnType));

	//As it would be a lot of effort at the moment to show that a loop works, testing will be done later!

	//whileInput->value(true);
	//ASSERT_THROW_INTERPRETER(m_executor.evaluate(graphical), RuntimeError);
}

#include <programGraph/ParameterAccessBlock.h>
TEST_F(TestExecutor, ParameterAccessBlock)
{
	auto input1 = Datatype::BOOLEAN;
	auto input2 = Datatype::DOUBLE;
	auto graphical = GraphicalFunction({ input1, input2 }, { input1, input2 });
	auto returnStatement = std::make_shared<ReturnBlock>(graphical);
	auto parameterAccess = std::make_shared<ParameterAccessBlock>(graphical);
	returnStatement->setInputConnections({ Connection(parameterAccess, 0), Connection(parameterAccess, 1) });
	graphical.setStatementBlocks({ returnStatement });
	graphical.setExpressionBlocks({ parameterAccess });

	auto inputValue1 = Value(false);
	auto inputValue2 = Value(2.0);
	EXPECT_EQ(std::vector<Value>({ inputValue1, inputValue2 }), m_executor.evaluate(graphical, {inputValue1, inputValue2}));
}

TEST_F(TestExecutor, ParameterCount)
{
	auto input1 = Datatype::BOOLEAN;
	auto input2 = Datatype::DOUBLE;
	auto graphical = GraphicalFunction({ input1, input2 }, { input1, input2 });
	auto returnStatement = std::make_shared<ReturnBlock>(graphical);
	auto parameterAccess = std::make_shared<ParameterAccessBlock>(graphical);
	returnStatement->setInputConnections({ Connection(parameterAccess, 0), Connection(parameterAccess, 1) });
	graphical.setStatementBlocks({ returnStatement });
	graphical.setExpressionBlocks({ parameterAccess });

	ASSERT_THROW_INTERPRETER(m_executor.evaluate(graphical, { Value(input1), Value(input2), Value(false) }), RuntimeError);
	ASSERT_NO_THROW(m_executor.evaluate(graphical, { Value(input1), Value(input2) }));
}

TEST_F(TestExecutor, ParameterTypes)
{
	auto input1 = Datatype::BOOLEAN;
	auto input2 = Datatype::DOUBLE;
	auto graphical = GraphicalFunction({ input1, input2 }, { input1, input2 });
	auto returnStatement = std::make_shared<ReturnBlock>(graphical);
	auto parameterAccess = std::make_shared<ParameterAccessBlock>(graphical);
	returnStatement->setInputConnections({ Connection(parameterAccess, 0), Connection(parameterAccess, 1) });
	graphical.setStatementBlocks({ returnStatement });
	graphical.setExpressionBlocks({ parameterAccess });

	ASSERT_THROW_INTERPRETER(m_executor.evaluate(graphical, { Value(input2), Value(input1) }), RuntimeError);
}

TEST_F(TestExecutor, ParametersArePassedToGraphicalFunction) 
{
	auto testType = Datatype::DOUBLE;

	auto graphical1 = GraphicalFunction({ testType }, { testType });
	auto returnStatement1 = std::make_shared<ReturnBlock>(graphical1);
	auto parameterAccess1 = std::make_shared<ParameterAccessBlock>(graphical1);
	returnStatement1->setInputConnection(0, Connection(parameterAccess1, 0));
	graphical1.setStatementBlocks({ returnStatement1 });
	graphical1.setExpressionBlocks({ parameterAccess1 });

	auto graphical2 = GraphicalFunction({ testType }, { testType });
	auto returnStatement2 = std::make_shared<ReturnBlock>(graphical2);
	auto functionCall2 = std::make_shared<FunctionBlock>(graphical1);
	auto parameterAccess2 = std::make_shared<ParameterAccessBlock>(graphical2);
	returnStatement2->setInputConnection(0, Connection(functionCall2, 0));
	functionCall2->setInputConnection(0, Connection(parameterAccess2, 0));

	graphical2.setStatementBlocks({ returnStatement2 });
	graphical2.setExpressionBlocks({ functionCall2, parameterAccess2 });
	
	auto testValue = 10.0;
	auto parameters = std::vector<Value>();
	parameters.emplace_back(testValue);
	ASSERT_EQ(parameters, m_executor.evaluate(graphical2, parameters));
}