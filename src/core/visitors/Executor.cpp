#include <visitors/Executor.h>
#include <programGraph/ReturnBlock.h>
#include <error/InternalError.h>
#include <programGraph/GraphicalFunction.h>
#include <programGraph/ExpressionStatement.h>
#include <error/RuntimeError.h>
#include <programGraph/VariableWriteBlock.h>
#include <programGraph/IfStatement.h>

Executor::Executor()
	: m_evaluator(*this)
{}

Executor::~Executor()
{

}

void Executor::visit(class GraphicalFunction& graphicalFunction)
{
	if (!graphicalFunction.statementBlocks().empty())
	{
		graphicalFunction.statementBlocks().front()->accept(*this);
	}
}

void Executor::visit(class FunctionBlock& functionBlock)
{
	throwExpressionError();
}

void Executor::visit(class Connection& connection)
{
	throwExpressionError();
}

void Executor::visit(class ValueBlock& valueBlock)
{
	throwExpressionError();
}

void Executor::visit(class PrimitiveFunction& primitiveFunction)
{
	throwExpressionError();
}

void Executor::visit(class ReturnBlock& returnBlock)
{
	auto returnValues = Return();
	auto connections = returnBlock.inputConnections();
	for (size_t i = 0; i < connections.size(); i++)
	{
		auto connection = connections.at(i);
		auto type = returnBlock.inputTypes().at(i);
		returnValues.m_values.emplace_back(m_evaluator.evaluateConnection(connection, type));
	}

	throw returnValues;
}

void Executor::visit(class ExpressionStatement& expressionStatement)
{
	m_executedStatements[&expressionStatement] = expressionStatement.expression().accept(m_evaluator);
	
	executeNext(expressionStatement);
}


void Executor::visit(class VariableReadBlock& variableReadBlock)
{
	throwExpressionError();
}

void Executor::visit(class VariableWriteBlock& variableWriteBlock)
{
	auto& currentStackFrame = m_callStack.top();
	auto inputConnection = variableWriteBlock.inputConnections().front();
	auto inputType = variableWriteBlock.inputTypes().front();
	auto variableValue = m_evaluator.evaluateConnection(inputConnection, inputType);

	currentStackFrame.at(variableWriteBlock.variableIdentifier()) = variableValue;
	m_executedStatements[&variableWriteBlock] = { variableValue };

	executeNext(variableWriteBlock);
}

void Executor::visit(class IfStatement& ifStatement)
{
	auto inputConnection = ifStatement.inputConnections().front();
	auto conditionValue = inputConnection.accept(m_evaluator).front();
	if (conditionValue.getBoolean())
	{
		executeNext(ifStatement, 0);
	}
	else
	{
		executeNext(ifStatement, 1);
	}
}

void Executor::executeNext(class StatementBlock& statement, size_t flowConnectionIndex)
{
	auto& statementConnection = statement.flowConnections().at(flowConnectionIndex);
	if (statementConnection.isConnected())
	{
		statementConnection.connectedStatement()->accept(*this);
	}
}

void Executor::executeNext(class StatementBlock& statement)
{
	executeNext(statement, 0);
}


std::vector<Value> Executor::evaluate(class GraphicalFunction& graphicalFunction)
{
	auto currentStackFrame = std::map<VariableIdentifier, Value>();
	for (const auto& id : graphicalFunction.variables())
	{
		currentStackFrame.emplace(id.first, Value(id.second));
	}
	m_callStack.push(std::move(currentStackFrame));

	try
	{
		graphicalFunction.accept(*this);
	}
	catch (const Return& returnValues)
	{
		return returnValues.m_values;
	}

	m_callStack.pop();

	if (!graphicalFunction.outputs().empty())
	{
		THROW_ERROR(RuntimeError, "graphical function which should return a value, did not reach a return statement!");
	}

	return {};
}

std::vector<Value> Executor::evaluate(class StatementBlock& statement)
{
	auto savedState = m_executedStatements.find(&statement);
	if (savedState == m_executedStatements.end())
	{
		THROW_ERROR(RuntimeError, "Forward data connection detected!");
	}
	return savedState->second;
}

Value Executor::variableValue(VariableIdentifier identifier)
{
	return m_callStack.top().at(identifier);
}

void Executor::throwExpressionError()
{
	THROW_ERROR(InternalError, "Executor cannot execute expressions!");
}

