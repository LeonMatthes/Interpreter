#include <visitors/TypeCheckResult.h>

TypeCheckResult::TypeCheckResult(bool success)
	: succeeded(success)
{

}

void TypeCheckResult::merge(TypeCheckResult result)
{
	succeeded = succeeded && result.succeeded;

	for (const auto& offender : result.offenders)
	{
		const auto& offendingFunction = offender.first;
		const auto& offendingBlocks = offender.second;
		
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

}

TypeCheckResult::operator bool() const
{
	return succeeded;
}
