#pragma once
#include <programGraph/GraphicalFunction.h>
#include <programGraph/Value.h>
#include <vector>

class Program
{
public:
	Program(GraphicalFunction::UPtr startFunction, std::vector<GraphicalFunction::UPtr> functions);
	Program(Program&& /*other*/); //For use with auto, do NOT use program after moving it!
	virtual ~Program() = default;

	std::vector<Value> run(std::vector<Value> parameters);

	std::vector<GraphicalFunction::UPtr>& functions();
	GraphicalFunction::UPtr& startFunction();
protected:
	std::vector<GraphicalFunction::UPtr> m_functions;
	GraphicalFunction::UPtr m_startFunction;
private:
};