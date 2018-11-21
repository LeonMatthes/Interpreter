#pragma once
#include <map>
#include <set>

class TypeCheckResult
{
public:
	TypeCheckResult(bool success);

	bool succeeded;
	std::map<class Function*, std::set<class Block*>> offenders;

	void merge(TypeCheckResult result);
	explicit operator bool() const;

};