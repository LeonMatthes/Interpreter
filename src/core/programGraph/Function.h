#pragma once

#include <programGraph/Datatype.h>
#include <vector>

class Function
{
public:
	Function() = default;
	Function(std::vector<Datatype> inputs, std::vector<Datatype> outputs);
	virtual ~Function() = default;

	std::vector<Datatype> outputs() const;
	std::vector<Datatype> inputs() const;
protected:
	const std::vector<Datatype> m_inputs;
	const std::vector<Datatype> m_outputs;
private:
};