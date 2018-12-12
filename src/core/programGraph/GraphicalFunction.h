#pragma once
#include <memory>
#include <programGraph/ExpressionBlock.h>
#include <programGraph/Function.h>
#include <programGraph/StatementBlock.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <visitors/Visitor.h>

using VariableIdentifier = std::string;

class GraphicalFunction : public Function
{
public:
	using UPtr = std::unique_ptr<GraphicalFunction>;

	GraphicalFunction(std::vector<Datatype> inputs, std::vector<Datatype> outputs);
	GraphicalFunction(GraphicalFunction&& other); //Move needed for auto
	GraphicalFunction(const GraphicalFunction&) = delete;
	GraphicalFunction();
	~GraphicalFunction() override;
	
	const std::vector<ExpressionBlock::Ptr>& expressionBlocks() const;
	void setExpressionBlocks(std::vector<ExpressionBlock::Ptr> blocks);

	const std::vector<StatementBlock::Ptr>& statementBlocks() const;
	void setStatementBlocks(std::vector<StatementBlock::Ptr> blocks);

	void addVariable(const VariableIdentifier& identifier, Datatype type);
	Datatype variableType(const VariableIdentifier& identifier) const;
	bool hasVariable(const VariableIdentifier& identifier) const;
	const std::unordered_map<VariableIdentifier, Datatype>& variables() const;


	OVERRIDE_VISITABLE;

protected:
	std::vector<ExpressionBlock::Ptr> m_expressionBlocks;
	std::vector<StatementBlock::Ptr> m_statementBlocks;

	std::unordered_map<VariableIdentifier, Datatype> m_variables;
private:
};