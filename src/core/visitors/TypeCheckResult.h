#pragma once
#include <map>
#include <set>

class TypeCheckResult final
{
public:
	TypeCheckResult();
	TypeCheckResult(bool success);
	TypeCheckResult(class Function* offendingFunction, class Block* offendingBlock);
	TypeCheckResult(class Function* offendingFunction, const std::set<class Block*>& offendingBlocks);

	bool succeeded;
	std::map<class Function*, std::set<class Block*>> offenders;

	void merge(const TypeCheckResult& result);
	operator bool() const;

private:
	void addOffenders(Function* offendingFunction, const std::set<Block*>& offendingBlocks);
};