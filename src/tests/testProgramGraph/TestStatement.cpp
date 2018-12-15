#include <gtest/gtest.h>

#include <programGraph/StatementBlock.h>
#include <error/Error.h>


class MockStatementBlock : public StatementBlock
{
public:
	using Ptr = std::shared_ptr<MockStatementBlock>;

	MockStatementBlock(size_t inputCount, size_t flowConnectionsCount = 1) 
		: StatementBlock(inputCount, flowConnectionsCount) 
	{}

        ~MockStatementBlock() override = default;

        std::vector<Datatype> inputTypes() const override
        {
		return std::vector<Datatype>(inputConnections().size());
	}

        std::vector<Datatype> outputTypes() const override
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

TEST(TestStatementBlock, flowConnectionsCount)
{
	MockStatementBlock statementBlock(0);
	EXPECT_EQ(1, statementBlock.flowConnectionsCount());

	statementBlock = MockStatementBlock(0, 0);
	EXPECT_EQ(0, statementBlock.flowConnectionsCount());
}

TEST(TestStatementBlock, flowConnections)
{
	MockStatementBlock statementBlock(0);
	EXPECT_EQ(1, statementBlock.flowConnections().size());
	EXPECT_FALSE(statementBlock.flowConnections().at(0).isConnected());

	statementBlock = MockStatementBlock(0, 0);
	EXPECT_EQ(0, statementBlock.flowConnections().size());
}

#include <programGraph/ProgramFlowConnection.h>
TEST(TestStatementBlock, setFlowConnections)
{
	auto statementBlock = MockStatementBlock(0, 1);
	EXPECT_THROW(statementBlock.setFlowConnections({}), Error::Ptr);
	
	auto secondStatement = std::make_shared<MockStatementBlock>(0);
	auto connections = std::vector<ProgramFlowConnection>({ ProgramFlowConnection(secondStatement) });
	statementBlock.setFlowConnections(connections);
	EXPECT_EQ(connections, statementBlock.flowConnections());
}

//////////////////////////////////////////////////////////////////////////
//						Test ProgramFlowConnection
//////////////////////////////////////////////////////////////////////////

TEST(TestProgramFlowConnection, isConnected)
{
	EXPECT_FALSE(ProgramFlowConnection().isConnected());
	
	MockStatementBlock::Ptr statement = std::make_shared<MockStatementBlock>(0);
	ProgramFlowConnection connection(statement);
	EXPECT_TRUE(connection.isConnected());
}

TEST(TestProgramFlowConnection, connected)
{
	MockStatementBlock::Ptr statement = std::make_shared<MockStatementBlock>(0);
	ProgramFlowConnection connection(statement);
	EXPECT_EQ(statement, connection.connectedStatement());

	EXPECT_THROW(ProgramFlowConnection().connectedStatement(), Error::Ptr);
}
