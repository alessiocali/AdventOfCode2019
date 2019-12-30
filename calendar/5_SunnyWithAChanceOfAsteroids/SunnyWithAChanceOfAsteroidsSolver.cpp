#include <SunnyWithAChanceOfAsteroidsSolver.h>

#include <IntcodeProgram.h>

#include <sstream>

std::uint32_t SunnyWithAChanceOfAsteroidsSolver::SolveProblemA() const
{
	return SolveWithInput(1);
}

std::uint32_t SunnyWithAChanceOfAsteroidsSolver::SolveProblemB() const
{
	return SolveWithInput(5);
}

std::uint32_t SunnyWithAChanceOfAsteroidsSolver::SolveWithInput(std::uint32_t input) const
{
	IntCodeComputer program(m_InputFilename);
	program.FeedInput(input);
	program.Execute();

	std::vector<int> output = GetOutputAsIntegers(program);

	return static_cast<std::uint32_t>(output.back());
}

std::vector<int> SunnyWithAChanceOfAsteroidsSolver::GetOutputAsIntegers(IntCodeComputer& program)
{
	std::vector<int> output;

	int temp;
	while (program.GetOutput(temp))
	{
		output.push_back(temp);
	}

	return output;
}