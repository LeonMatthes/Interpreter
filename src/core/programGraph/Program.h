#include <programGraph/GraphicalFunction.h>
#include <vector>
#include <programGraph/Value.h>

class Program
{
public:
	Program(GraphicalFunction::UPtr startFunction, std::vector<GraphicalFunction::UPtr> functions);
	Program(Program&&);
	virtual ~Program() = default;

	std::vector<Value> run(std::vector<Value> parameters);
protected:
	std::vector<GraphicalFunction::UPtr> m_functions;
	GraphicalFunction::UPtr m_startFunction;
private:
};