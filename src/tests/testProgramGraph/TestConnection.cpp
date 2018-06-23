#include <gtest/gtest.h>
#include <programGraph/Connection.h>
#include <programGraph/Function.h>
#include <programGraph/Block.h>


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