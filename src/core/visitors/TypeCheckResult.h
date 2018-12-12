#pragma once
#include <map>
#include <set>

class TypeCheckResult final {
public:
    TypeCheckResult();
    TypeCheckResult(class Function* offendingFunction, class Block* offendingBlock);
    explicit TypeCheckResult(class Function* offendingFunction, const std::set<class Block*>& offendingBlocks);

    bool succeeded;
    std::map<class Function*, std::set<class Block*>> offenders;

    void merge(const TypeCheckResult& result);

    // These conversions are allowed
    TypeCheckResult(bool success); // NOLINT(google-explicit-constructor)
    operator bool() const; // NOLINT(google-explicit-constructor)

private:
    void addOffenders(Function* offendingFunction, const std::set<Block*>& offendingBlocks);
};
