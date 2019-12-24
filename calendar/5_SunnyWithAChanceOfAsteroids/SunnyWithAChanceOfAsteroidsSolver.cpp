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
	std::shared_ptr<std::iostream> outputStringStream = std::make_shared<std::stringstream>();
	std::shared_ptr<std::iostream> inputStringStream = std::make_shared<std::stringstream>();

	*inputStringStream << input << std::endl;

	IntCodeProgram program(m_InputFilename);
	program.SetOutputStream(outputStringStream);
	program.SetInputStream(inputStringStream);
	program.Execute();

	std::vector<int> output = GetOutputAsIntegers(*outputStringStream);

	return static_cast<std::uint32_t>(output.back());
}

std::vector<int> SunnyWithAChanceOfAsteroidsSolver::GetOutputAsIntegers(std::istream& inputStream)
{
	std::vector<int> output;

	int temp;
	while (inputStream >> temp)
	{
		output.push_back(temp);
	}

	return output;
}