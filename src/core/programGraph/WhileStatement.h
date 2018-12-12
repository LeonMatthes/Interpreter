#pragma once

#include <programGraph/StatementBlock.h>

class WhileStatement : public StatementBlock {
public:
    WhileStatement();
    ~WhileStatement() override = default;

    std::vector<Datatype> inputTypes() const override;
    std::vector<Datatype> outputTypes() const override;

    OVERRIDE_VISITABLE;

protected:
private:
};
