#pragma once

#include <programGraph/Datatype.h>
#include <vector>
#include <visitors/Visitor.h>

class Function
{
public:
	Function();
	Function(std::vector<Datatype> inputs, std::vector<Datatype> outputs);
	virtual ~Function();

	std::vector<Datatype> outputs() const;
	std::vector<Datatype> inputs() const;

	MAKE_VISITABLE;

protected:
	const std::vector<Datatype> m_inputs;
	const std::vector<Datatype> m_outputs;
private:
};