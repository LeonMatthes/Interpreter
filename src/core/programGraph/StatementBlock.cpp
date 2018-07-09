#include <programGraph/StatementBlock.h>


StatementBlock::StatementBlock(size_t inputCount, size_t flowConnectionsCount /*= 1*/)
	: Block(inputCount)
{}

StatementBlock::~StatementBlock()
{}
