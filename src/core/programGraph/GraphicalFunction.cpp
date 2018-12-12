#include <error/InternalError.h>
#include <programGraph/GraphicalFunction.h>
#include <utility/Literals.h>

#include <utility>

GraphicalFunction::GraphicalFunction()
    = default;

GraphicalFunction::GraphicalFunction(std::vector<Datatype> inputs, std::vector<Datatype> outputs)
    : Function(std::move(inputs), std::move(outputs))
{
}

GraphicalFunction::GraphicalFunction(GraphicalFunction&& other)
    : Function(other)
    , m_expressionBlocks(std::move(other.m_expressionBlocks))
    , m_statementBlocks(std::move(other.m_statementBlocks))
{
}

GraphicalFunction::~GraphicalFunction()
    = default;

const std::vector<ExpressionBlock::Ptr>& GraphicalFunction::expressionBlocks() const
{
    return m_expressionBlocks;
}

void GraphicalFunction::setExpressionBlocks(std::vector<ExpressionBlock::Ptr> blocks)
{
    m_expressionBlocks = std::move(blocks);
}

const std::vector<StatementBlock::Ptr>& GraphicalFunction::statementBlocks() const
{
    return m_statementBlocks;
}

void GraphicalFunction::setStatementBlocks(std::vector<StatementBlock::Ptr> blocks)
{
    m_statementBlocks = std::move(blocks);
}

void GraphicalFunction::addVariable(const VariableIdentifier& identifier, Datatype type)
{
    m_variables[identifier] = type;
}

Datatype GraphicalFunction::variableType(const VariableIdentifier& identifier) const
{
    if (!hasVariable(identifier)) {
        THROW_ERROR(InternalError, "Tried to access variable type of variable: "_s + identifier + ", which does not exist!");
    }
    return m_variables.at(identifier);
}

bool GraphicalFunction::hasVariable(const VariableIdentifier& identifier) const
{
    return m_variables.find(identifier) != m_variables.end();
}

const std::unordered_map<VariableIdentifier, Datatype>& GraphicalFunction::variables() const
{
    return m_variables;
}
