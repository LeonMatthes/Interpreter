#include <visitors/SteppingExecutor.h>

SteppingExecutor::SteppingExecutor(const GraphicalFunction& functionToExecute)
    : m_instructionPointer { functionToExecute.statementBlocks().front() }
    , m_evaluator { *this }
{
}
