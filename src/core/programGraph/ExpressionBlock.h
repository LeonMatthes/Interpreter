#pragma once
#include <programGraph/Block.h>
#include <visitors/Visitor.h>

//this class exists to differentiate Expressions from Statements
class ExpressionBlock : public Block {
public:
    using Ptr = std::shared_ptr<ExpressionBlock>;

    explicit ExpressionBlock(size_t inputCount);

    ~ExpressionBlock() override;

    MAKE_VISITABLE_VIRTUAL;

protected:
private:
};
