#pragma once
#include <programGraph/StatementBlock.h>
#include <programGraph/Value.h>
#include <visitors/Evaluator.h>
#include <visitors/Executor.h>

class SteppingExecutor : public Executor {
    struct StackFrame {
        bool popFunctionStack = false;
        StatementBlock::Ptr instructionPointer;
    };

    struct FunctionStackFrame {
        std::unordered_map<VariableIdentifier, Value> variables;
        std::vector<Value> parameters;
        std::unordered_map<StatementBlock::Ptr, std::vector<Value>> executedStatements;
    };

public:
    SteppingExecutor(const class GraphicalFunction& functionToExecute);
    ~SteppingExecutor() override = default;

protected:
    Evaluator m_evaluator;
    StatementBlock::Ptr m_instructionPointer;
    std::stack<StackFrame, std::vector<StackFrame>> m_returnStack;
    std::stack<FunctionStackFrame, std::vector<FunctionStackFrame>> m_functionStack;

private:
};
