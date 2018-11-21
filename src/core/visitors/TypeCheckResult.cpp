#include <visitors/TypeCheckResult.h>

TypeCheckResult::TypeCheckResult() 
	: TypeCheckResult(true)
{

}

TypeCheckResult::TypeCheckResult(bool success)
	: succeeded(success)
{

}

TypeCheckResult::TypeCheckResult(class Function* offendingFunction, class Block* offendingBlock)
	: TypeCheckResult(offendingFunction, std::set<class Block*>({offendingBlock}))
{

}

TypeCheckResult::TypeCheckResult(class Function* offendingFunction, const std::set<class Block*>& offendingBlocks)
	: succeeded(false)
{
	addOffenders(offendingFunction, offendingBlocks);
}

void TypeCheckResult::merge(const TypeCheckResult& result)
{
	succeeded = succeeded && result.succeeded;

	for (const auto& offender : result.offenders)
	{
		const auto& offendingFunction = offender.first;
		const auto& offendingBlocks = offender.second;
		
		addOffenders(offendingFunction, offendingBlocks);
	}

}

TypeCheckResult::operator bool() const
{
	return succeeded;
}

void TypeCheckResult::addOffenders(Function* offendingFunction, const std::set<Block*>& offendingBlocks)
{
	auto offenderIt = offenders.find(offendingFunction);
	if (offenderIt == offenders.end())
	{
		offenders[offendingFunction] = offendingBlocks;
	}
	else
	{
		offenders.at(offendingFunction).insert(offendingBlocks.begin(), offendingBlocks.end());
	}
}
