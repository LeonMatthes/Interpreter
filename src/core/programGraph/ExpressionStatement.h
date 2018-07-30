#pragma once
#include <programGraph/StatementBlock.h>
#include <memory>

class ExpressionStatement : public StatementBlock
{
public:
	template<class T, class... Args>
	static std::shared_ptr<ExpressionStatement> make_shared(Args&&... args)
	{
		auto expression = std::unique_ptr<ExpressionBlock>(new T(std::forward<Args>(args)... /*copied from make_shared in <memory>*/));
		return std::make_shared<ExpressionStatement>(std::move(expression));
	}

	ExpressionStatement(std::unique_ptr<class ExpressionBlock> expression);
	ExpressionStatement(const ExpressionStatement&) = delete;
	virtual ~ExpressionStatement();

	virtual std::vector<Datatype> inputTypes() const override;
	virtual std::vector<Datatype> outputTypes() const override;

	virtual void setInputConnections(std::vector<Connection> val) override;

	class ExpressionBlock& expression();

	MAKE_VISITABLE;
protected:
	
	std::unique_ptr<class ExpressionBlock> m_expression;
private:
};