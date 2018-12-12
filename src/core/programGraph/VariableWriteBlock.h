#pragma once

#include <programGraph/GraphicalFunction.h>
#include <programGraph/StatementBlock.h>
#include <visitors/Visitor.h>

class VariableWriteBlock : public StatementBlock {
public:
    VariableWriteBlock(class GraphicalFunction& owningFunction, VariableIdentifier identifier);
    ~VariableWriteBlock() override = default;

    OVERRIDE_VISITABLE;

    std::vector<Datatype> inputTypes() const override;
    std::vector<Datatype> outputTypes() const override;

    const VariableIdentifier& variableIdentifier() const;

protected:
    class GraphicalFunction& m_owningFunction;
    VariableIdentifier m_identifier;

private:
};
