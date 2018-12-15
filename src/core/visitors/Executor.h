#pragma once
#include <programGraph/GraphicalFunction.h>
#include <visitors/Visitor.h>

class Executor : public Visitor<void> {
public:
    virtual ~Executor() = default;

    virtual std::vector<Value> evaluate(class StatementBlock& statement) = 0;
    virtual std::vector<Value> evaluate(class GraphicalFunction& graphicalFunction) = 0;
    virtual std::vector<Value> evaluate(class GraphicalFunction& graphicalFunction, std::vector<Value> parameters) = 0;
    virtual Value variableValue(const VariableIdentifier& identifier) = 0;
    virtual std::vector<Value> parameters() const = 0;
};
