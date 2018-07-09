#include <gtest/gtest.h>

#include <programGraph/StatementBlock.h>


class MockStatementBlock : public StatementBlock
{
public:
	using Ptr = std::shared_ptr<MockStatementBlock>;

	MockStatementBlock(size_t inputCount) 
		: StatementBlock(inputCount) 
	{}
	
	virtual ~MockStatementBlock() = default;

	virtual std::vector<Datatype> inputTypes() const override
	{
		return std::vector<Datatype>(inputConnections().size());
	}

	virtual std::vector<Datatype> outputTypes() const override
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	MOCK_VISITABLE;
protected:
	
private:
};

TEST(TestStatementBlock, inputCount)
{
	MockStatementBlock statementBlock(10);
	EXPECT_EQ(statementBlock.inputCount(), 10);

	MockStatementBlock secondStatementBlock(1);
	EXPECT_EQ(secondStatementBlock.inputCount(), 1);
}




//////////////////////////////////////////////////////////////////////////
//						Test ProgramFlowConnection
//////////////////////////////////////////////////////////////////////////
#include <programGraph/ProgramFlowConnection.h>

TEST(TestProgramFlowConnection, isConnected)
{
	EXPECT_FALSE(ProgramFlowConnection().isConnected());
	
	MockStatementBlock::Ptr statement = std::make_shared<MockStatementBlock>(0);
	ProgramFlowConnection connection(statement);
	EXPECT_TRUE(connection.isConnected());
}
