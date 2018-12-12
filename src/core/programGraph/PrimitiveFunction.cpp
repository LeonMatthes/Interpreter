#include <programGraph/PrimitiveFunction.h>

#include <utility>

//////////////////////////////////////////////////////////////////////////
//					Math Primitives
//////////////////////////////////////////////////////////////////////////
PrimitiveFunction PrimitiveFunction::add(
    "+",
    std::vector<Datatype>({ Datatype::DOUBLE, Datatype::DOUBLE }),
    std::vector<Datatype>({ Datatype::DOUBLE }),
    [](std::vector<Value> inputs) {
        double sum = inputs.at(0).getDouble() + inputs.at(1).getDouble();
        return std::vector<Value>({ Value(sum) });
    });

PrimitiveFunction PrimitiveFunction::subtract(
    "-",
    std::vector<Datatype>({ Datatype::DOUBLE, Datatype::DOUBLE }),
    std::vector<Datatype>({ Datatype::DOUBLE }),
    [](std::vector<Value> inputs) {
        double result = inputs.at(0).getDouble() - inputs.at(1).getDouble();
        return std::vector<Value>({ Value(result) });
    });

PrimitiveFunction PrimitiveFunction::multiply(
    "*",
    std::vector<Datatype>({ Datatype::DOUBLE, Datatype::DOUBLE }),
    std::vector<Datatype>({ Datatype::DOUBLE }),
    [](std::vector<Value> inputs) {
        double result = inputs.at(0).getDouble() * inputs.at(1).getDouble();
        return std::vector<Value>({ Value(result) });
    });

PrimitiveFunction PrimitiveFunction::divide(
    "/",
    std::vector<Datatype>({ Datatype::DOUBLE, Datatype::DOUBLE }),
    std::vector<Datatype>({ Datatype::DOUBLE }),
    [](std::vector<Value> inputs) {
        double result = inputs.at(0).getDouble() / inputs.at(1).getDouble();
        return std::vector<Value>({ Value(result) });
    });

//////////////////////////////////////////////////////////////////////////
//				Comparison Primitives
//////////////////////////////////////////////////////////////////////////
PrimitiveFunction PrimitiveFunction::smaller(
    "<",
    std::vector<Datatype>({ Datatype::DOUBLE, Datatype::DOUBLE }),
    std::vector<Datatype>({ Datatype::BOOLEAN }),
    [](std::vector<Value> inputs) {
        bool result = inputs.at(0).getDouble() < inputs.at(1).getDouble();
        return std::vector<Value>({ Value(result) });
    });

//////////////////////////////////////////////////////////////////////////
//				Boolean Primitives
//////////////////////////////////////////////////////////////////////////
PrimitiveFunction PrimitiveFunction::logicalAnd(
    "and",
    std::vector<Datatype>({ Datatype::BOOLEAN, Datatype::BOOLEAN }),
    std::vector<Datatype>({ Datatype::BOOLEAN }),
    [](std::vector<Value> inputs) {
        bool result = inputs.at(0).getBoolean() && inputs.at(1).getBoolean();
        return std::vector<Value>({ Value(result) });
    });

PrimitiveFunction PrimitiveFunction::logicalOr(
    "or",
    std::vector<Datatype>({ Datatype::BOOLEAN, Datatype::BOOLEAN }),
    std::vector<Datatype>({ Datatype::BOOLEAN }),
    [](std::vector<Value> inputs) {
        bool result = inputs.at(0).getBoolean() || inputs.at(1).getBoolean();
        return std::vector<Value>({ Value(result) });
    });

PrimitiveFunction PrimitiveFunction::logicalNot(
    "not",
    std::vector<Datatype>({ Datatype::BOOLEAN }),
    std::vector<Datatype>({ Datatype::BOOLEAN }),
    [](std::vector<Value> inputs) {
        bool result = !inputs.front().getBoolean();
        return std::vector<Value>({ Value(result) });
    });

PrimitiveFunction PrimitiveFunction::castBoolToDouble(
    "Cast",
    std::vector<Datatype>({ Datatype::BOOLEAN }),
    std::vector<Datatype>({ Datatype::DOUBLE }),
    [](std::vector<Value> inputs) {
        auto result = inputs.front().getBoolean() ? 1.0 : 0.0;
        return std::vector<Value>({ Value(result) });
    });

PrimitiveFunction PrimitiveFunction::castDoubleToBool(
    "Cast",
    std::vector<Datatype>({ Datatype::DOUBLE }),
    std::vector<Datatype>({ Datatype::BOOLEAN }),
    [](std::vector<Value> inputs) {
        bool result = inputs.front().getDouble() != 0.0;
        return std::vector<Value>(static_cast<std::vector<class Value, class std::allocator<class Value>>::size_type>(result));
    });

PrimitiveFunction::PrimitiveFunction(std::string name, std::vector<Datatype> inputTypes, std::vector<Datatype> outputTypes, std::function<std::vector<Value>(std::vector<Value>)> executeFunction)
    : Function(std::move(inputTypes), std::move(outputTypes))
    , m_executeFunction { std::move(std::move(executeFunction)) }
    , m_name { std::move(std::move(name)) }
    , m_statement { false }
{
}

std::vector<Value> PrimitiveFunction::operator()(std::vector<Value> inputs) const
{
    return m_executeFunction(std::move(inputs));
}

std::string PrimitiveFunction::name() const
{
    return m_name;
}

bool PrimitiveFunction::isStatement() const
{
    return m_statement;
}
