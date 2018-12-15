#include <gtest/gtest.h>
#include <visitors/Evaluator.h>
#include <visitors/RecursiveExecutor.h>
#include <testProgramGraph/MockFunction.h>
#include <programGraph/Value.h>
#include <programGraph/ValueBlock.h>
#include <programGraph/GraphicalFunction.h>
#include <programGraph/FunctionBlock.h>
#include <programGraph/PrimitiveFunction.h>
#include <error/Error.h>


class MockExecutor : public RecursiveExecutor
{
public:
    void visit(class GraphicalFunction& graphicalFunction) override
    {
        throw std::logic_error("The method or operation is not implemented.");
	}

        void visit(class FunctionBlock& functionBlock) override
        {
		throw std::logic_error("The method or operation is not implemented.");
	}

        void visit(class Connection& connection) override
        {
		throw std::logic_error("The method or operation is not implemented.");
	}

        void visit(class ValueBlock& valueBlock) override
        {
		throw std::logic_error("The method or operation is not implemented.");
	}

        void visit(class PrimitiveFunction& primitiveFunction) override
        {
		throw std::logic_error("The method or operation is not implemented.");
	}
};

class TestEvaluator : public ::testing::Test
{
public:
	MockExecutor m_executor;
	Evaluator m_evaluator;
	
	TestEvaluator()
		: m_executor(MockExecutor())
		, m_evaluator(m_executor)
	{}
protected:
private:
};


TEST_F(TestEvaluator, ValueBlock)
{
	Value value(false);
	std::vector<Value> values({ value });
	ValueBlock block(value);
	EXPECT_EQ(values, block.accept(m_evaluator));
} 

TEST_F(TestEvaluator, PrimitiveFunction)
{
	m_evaluator.pushParameters({ Value(2.0), Value(1.0) });
	EXPECT_EQ(Value(1.0), PrimitiveFunction::subtract.accept(m_evaluator).at(0));
}

TEST_F(TestEvaluator, Connection)
{
    ValueBlock::Ptr block = std::make_shared<ValueBlock>(Value(false));
    Connection connection(block, 0);
    EXPECT_EQ(Value(false), connection.accept(m_evaluator).at(0));

    block = std::make_shared<ValueBlock>(Value(true));
    connection = Connection(block, 0);
    EXPECT_EQ(Value(true), connection.accept(m_evaluator).at(0));
}

TEST_F(TestEvaluator, FunctionBlock)
{
	FunctionBlock block(PrimitiveFunction::subtract);
	//Unconnected block uses default for input types
	EXPECT_EQ(Value(0.0), block.accept(m_evaluator).at(0));
	
	ValueBlock::Ptr firstValue = std::make_shared<ValueBlock>(Value(2.0));
	ValueBlock::Ptr secondValue = std::make_shared<ValueBlock>(Value(1.0));
	block.setInputConnections({ Connection(firstValue, 0), Connection(secondValue, 0) });
	EXPECT_EQ(Value(1.0), block.accept(m_evaluator).at(0));

	block.setInputConnections({ Connection(firstValue, 0), Connection() });
	EXPECT_EQ(Value(2.0), block.accept(m_evaluator).at(0));
}