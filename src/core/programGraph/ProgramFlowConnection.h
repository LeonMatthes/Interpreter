#pragma once

#include <memory>

class ProgramFlowConnection {
public:
    explicit ProgramFlowConnection(const std::shared_ptr<class StatementBlock>& statement);
    ProgramFlowConnection();
    virtual ~ProgramFlowConnection();

    bool isConnected() const;
    std::shared_ptr<class StatementBlock> connectedStatement() const;

    bool operator==(const ProgramFlowConnection& other) const;

protected:
    std::weak_ptr<class StatementBlock> m_statement;

private:
};
