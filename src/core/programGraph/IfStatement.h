#pragma once

#include <programGraph/StatementBlock.h>

class IfStatement : public StatementBlock {
public:
    IfStatement();
    ~IfStatement() override;

    std::vector<Datatype> inputTypes() const override;
    std::vector<Datatype> outputTypes() const override;

    OVERRIDE_VISITABLE;

protected:
private:
};
