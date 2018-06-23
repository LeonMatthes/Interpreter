#include <gtest/gtest.h>
#include <programGraph/Connection.h>
#include <programGraph/Function.h>
#include <programGraph/Block.h>
#include <error/Error.h>

TEST(TestConnection, defaultConnectionIsEmpty)
{
	Connection connection;
	EXPECT_FALSE(connection.isConnected());
}

TEST(TestConnection, connectionInitialization)
{
	Function function({}, {Datatype::DOUBLE, Datatype::DOUBLE});
	Block block(function);

	size_t connectedOutput = 1;
	Connection connection(block, connectedOutput);
	ASSERT_TRUE(connection.isConnected());
	EXPECT_EQ(&block, &connection.connectedBlock());
	EXPECT_EQ(connectedOutput, connection.connectedOutput());
}

TEST(TestConnection, disconnectedConnectionThrowsErrors)
{
	Connection connection;
	EXPECT_THROW(connection.connectedBlock(), Error::Ptr);
	EXPECT_THROW(connection.connectedOutput(), Error::Ptr);
}

TEST(TestConnection, invalidConnectionCreation)
{
	Function function({}, {});
	Block block(function);
	size_t connectedOutput = 0;
	EXPECT_THROW(Connection connection(block, connectedOutput), Error::Ptr);
}