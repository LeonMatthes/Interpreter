#pragma once

#include <memory>

class ProgramFlowConnection
{
public:
	ProgramFlowConnection(std::shared_ptr<class StatementBlock> statement);
	ProgramFlowConnection();
	virtual ~ProgramFlowConnection();

	bool isConnected() const;
	std::shared_ptr<class StatementBlock> connectedStatement();

protected:
	std::weak_ptr<class StatementBlock> m_statement;
private:
};