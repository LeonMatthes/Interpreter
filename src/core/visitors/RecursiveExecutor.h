#pragma once

#include <map>
#include <programGraph/GraphicalFunction.h>
#include <programGraph/Value.h>
#include <stack>
#include <unordered_map>
#include <vector>
#include <visitors/Evaluator.h>
#include <visitors/Visitor.h>
#include <visitors/Executor.h>

struct Return {
    std::vector<Value> m_values;
};

struct StackFrame {
    std::map<VariableIdentifier, Value> m_variables;
    std::vector<Value> m_parameters;
};

class RecursiveExecutor : public Executor {
public:
    RecursiveExecutor();
    ~RecursiveExecutor() override;

    void visit(class GraphicalFunction& graphicalFunction) override;
    void visit(class FunctionBlock& functionBlock) override;
    void visit(class Connection& connection) override;
    void visit(class ValueBlock& valueBlock) override;
    void visit(class PrimitiveFunction& primitiveFunction) override;
    void visit(class VariableReadBlock& variableReadBlock) override;
    void visit(class ParameterAccessBlock& parameterAccess) override;

    void visit(class ReturnBlock& returnBlock) override;
    void visit(class ExpressionStatement& expressionStatement) override;
    void visit(class VariableWriteBlock& variableWriteBlock) override;
    void visit(class IfStatement& ifStatement) override;
    void visit(class WhileStatement& whileStatement) override;

    bool executeNext(class StatementBlock& statement);
    bool executeNext(class StatementBlock& statement, size_t flowConnectionIndex);

    std::vector<Value> evaluate(class StatementBlock& statement) override;
    std::vector<Value> evaluate(class GraphicalFunction& graphicalFunction) override;
    std::vector<Value> evaluate(class GraphicalFunction& graphicalFunction, std::vector<Value> parameters) override;
    Value variableValue(const VariableIdentifier& identifier) override;
    std::vector<Value> parameters() const override;

protected:
    Evaluator m_evaluator;
    std::unordered_map<class StatementBlock*, std::vector<Value>> m_executedStatements;
    std::stack<StackFrame, std::vector<StackFrame>> m_callStack;

    void throwExpressionError();

private:
};
