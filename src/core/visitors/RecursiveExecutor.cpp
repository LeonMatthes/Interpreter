#include <algorithm>
#include <error/InternalError.h>
#include <error/RuntimeError.h>
#include <programGraph/ExpressionStatement.h>
#include <programGraph/GraphicalFunction.h>
#include <programGraph/IfStatement.h>
#include <programGraph/ReturnBlock.h>
#include <programGraph/VariableWriteBlock.h>
#include <programGraph/WhileStatement.h>
#include <utility/Literals.h>
#include <visitors/RecursiveExecutor.h>

RecursiveExecutor::RecursiveExecutor()
    : m_evaluator(*this)
{
}

RecursiveExecutor::~RecursiveExecutor()
    = default;

void RecursiveExecutor::visit(class GraphicalFunction& graphicalFunction)
{
    if (!graphicalFunction.statementBlocks().empty()) {
        graphicalFunction.statementBlocks().front()->accept(*this);
    }
}

void RecursiveExecutor::visit(class FunctionBlock& /*functionBlock*/)
{
    throwExpressionError();
}

void RecursiveExecutor::visit(class Connection& /*connection*/)
{
    throwExpressionError();
}

void RecursiveExecutor::visit(class ValueBlock& /*valueBlock*/)
{
    throwExpressionError();
}

void RecursiveExecutor::visit(class PrimitiveFunction& /*primitiveFunction*/)
{
    throwExpressionError();
}

void RecursiveExecutor::visit(class ReturnBlock& returnBlock)
{
    auto returnValues = Return();
    auto connections = returnBlock.inputConnections();
    for (size_t i = 0; i < connections.size(); i++) {
        auto connection = connections.at(i);
        auto type = returnBlock.inputTypes().at(i);
        returnValues.m_values.emplace_back(m_evaluator.evaluateConnection(connection, type));
    }

    throw returnValues;
}

void RecursiveExecutor::visit(class ExpressionStatement& expressionStatement)
{
    m_executedStatements[&expressionStatement] = expressionStatement.expression().accept(m_evaluator);

    executeNext(expressionStatement);
}

void RecursiveExecutor::visit(class VariableReadBlock& /*variableReadBlock*/)
{
    throwExpressionError();
}

void RecursiveExecutor::visit(class VariableWriteBlock& variableWriteBlock)
{
    auto& currentStackFrame = m_callStack.top();
    auto inputConnection = variableWriteBlock.inputConnections().front();
    auto inputType = variableWriteBlock.inputTypes().front();
    auto variableValue = m_evaluator.evaluateConnection(inputConnection, inputType);

    currentStackFrame.m_variables.at(variableWriteBlock.variableIdentifier()) = variableValue;
    m_executedStatements[&variableWriteBlock] = { variableValue };

    executeNext(variableWriteBlock);
}

void RecursiveExecutor::visit(class IfStatement& ifStatement)
{
    auto inputConnection = ifStatement.inputConnections().front();
    auto conditionType = ifStatement.inputTypes().front();
    auto conditionValue = m_evaluator.evaluateConnection(inputConnection, conditionType);

    if (conditionValue.getBoolean()) {
        executeNext(ifStatement, 0);
    } else {
        executeNext(ifStatement, 1);
    }
}

void RecursiveExecutor::visit(class WhileStatement& whileStatement)
{
    auto conditionConnection = whileStatement.inputConnections().front();
    auto conditionType = whileStatement.inputTypes().front();
    while (m_evaluator.evaluateConnection(conditionConnection, conditionType).getBoolean()) {
        executeNext(whileStatement, 1);
    }

    executeNext(whileStatement);
}

void RecursiveExecutor::visit(class ParameterAccessBlock& /*parameterAccess*/)
{
    throwExpressionError();
}

bool RecursiveExecutor::executeNext(class StatementBlock& statement, size_t flowConnectionIndex)
{
    auto& statementConnection = statement.flowConnections().at(flowConnectionIndex);
    auto connected = statementConnection.isConnected();
    if (connected) {
        statementConnection.connectedStatement()->accept(*this);
    }
    return connected;
}

bool RecursiveExecutor::executeNext(class StatementBlock& statement)
{
    return executeNext(statement, 0);
}

std::vector<Value> RecursiveExecutor::evaluate(class GraphicalFunction& graphicalFunction)
{
    const auto& parameterTypes = graphicalFunction.inputs();
    auto parameters = std::vector<Value>(parameterTypes.size());
    std::transform(parameterTypes.begin(), parameterTypes.end(), parameters.begin(), [](Datatype type) {
        return Value(type);
    });

    return evaluate(graphicalFunction, parameters);
}

std::vector<Value> RecursiveExecutor::evaluate(class GraphicalFunction& graphicalFunction, std::vector<Value> parameters)
{
    if (parameters.size() != graphicalFunction.inputs().size()) {
        THROW_ERROR(RuntimeError, "Incorrect number of arguments pushed to function!", &graphicalFunction);
    }

    for (size_t i = 0; i < parameters.size(); i++) {
        const auto& parameter = parameters.at(i);
        if (parameter.type() != graphicalFunction.inputs().at(i)) {
            THROW_ERROR(RuntimeError, "Incorrect parameter type at parameter index: "_s + std::to_string(i), &graphicalFunction);
        }
    }

    auto currentStackFrame = StackFrame();
    currentStackFrame.m_parameters = parameters;
    auto& variables = currentStackFrame.m_variables;
    //Initialize all variables with their datatypes default Value
    for (const auto& id : graphicalFunction.variables()) {
        variables.emplace(id.first, Value(id.second));
    }

    m_callStack.push(std::move(currentStackFrame));

    try {
        graphicalFunction.accept(*this);
    } catch (const Return& returnValues) {
        m_callStack.pop();
        return returnValues.m_values;
    }

    m_callStack.pop();

    if (!graphicalFunction.outputs().empty()) {
        THROW_ERROR(RuntimeError, "graphical function which should return a value, did not reach a return statement!", &graphicalFunction);
    }

    return {};
}

std::vector<Value> RecursiveExecutor::evaluate(class StatementBlock& statement)
{
    auto savedState = m_executedStatements.find(&statement);
    if (savedState == m_executedStatements.end()) {
        THROW_ERROR(RuntimeError, "Forward data connection detected!", nullptr, &statement);
    }
    return savedState->second;
}

Value RecursiveExecutor::variableValue(const VariableIdentifier& identifier)
{
    return m_callStack.top().m_variables.at(identifier);
}

std::vector<Value> RecursiveExecutor::parameters() const
{
    return m_callStack.top().m_parameters;
}

void RecursiveExecutor::throwExpressionError()
{
    THROW_ERROR(InternalError, "Executor cannot execute expressions!");
}
