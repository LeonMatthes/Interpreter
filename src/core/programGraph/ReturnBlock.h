#pragma once

#include <programGraph/StatementBlock.h>

class ReturnBlock : public StatementBlock {
public:
    ReturnBlock() = delete;
    explicit ReturnBlock(class Function& function);
    ~ReturnBlock() override;

    std::vector<Datatype> inputTypes() const override;
    std::vector<Datatype> outputTypes() const override;

    OVERRIDE_VISITABLE;

protected:
    Function& m_function;

private:
};
