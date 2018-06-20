#include "gtest/gtest.h"
#include <test.h>

TEST(TestCase1, testFib)
{
	ASSERT_EQ(fib(0), 0);
	ASSERT_EQ(fib(1), 1);
	ASSERT_EQ(fib(2), 1);
	ASSERT_EQ(fib(3), 2);
	ASSERT_EQ(fib(4), 3);
	ASSERT_EQ(fib(5), 5);
	ASSERT_EQ(fib(6), 8);
}