#pragma once
#include <memory>
#include <programGraph/Block.h>
#include <programGraph/ProgramFlowConnection.h>

class StatementBlock : public Block {
public:
    using Ptr = std::shared_ptr<StatementBlock>;

    ~StatementBlock() override;
    size_t flowConnectionsCount();

    const std::vector<ProgramFlowConnection>& flowConnections();
    void setFlowConnections(const std::vector<ProgramFlowConnection>& connections);
    void setFlowConnection(size_t index, const ProgramFlowConnection& connection);

protected:
    explicit StatementBlock(size_t inputCount, size_t flowConnectionsCount);

    std::vector<ProgramFlowConnection> m_flowConnections;

private:
};
