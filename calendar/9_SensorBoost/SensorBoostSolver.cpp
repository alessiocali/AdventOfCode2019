#include <SensorBoostSolver.h>

IntCodeValue SensorBoostSolver::SolveWithInput(IntCodeValue input) const
{
	IntCodeComputer computer(m_InputFileName);
	if (!computer.IsValid())
	{
		std::cerr << "Error: can't open file " << m_InputFileName << std::endl;
		return 0;
	}

	IntCodeValue output;
	computer.FeedInput(input);
	computer.Execute();
	computer.GetOutput(output);

	return output;
}