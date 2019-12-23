#include <1202ProgramAlarmSolver.h>

#include <IntcodeProgram.h>

std::uint32_t _1202ProgramAlarmSolver::SolveProblemA() const
{
	IntcodeProgram program(m_ProgramFilename, { 12, 2 });
	if (!program.IsValid())
	{
		std::cerr << "Error: couldn't process input file " << m_ProgramFilename << std::endl;
		return 0;
	}

	program.Execute();
	return program.GetOpCodeAt(0);
}

std::uint32_t _1202ProgramAlarmSolver::SolveProblemB() const
{
	constexpr std::uint32_t Solution = 19690720;

	// Brute force time!
	for (std::uint32_t noun = 0; noun < 100; noun++)
	{
		for (std::uint32_t verb = 0; verb < 100; verb++)
		{
			IntcodeProgram program(m_ProgramFilename, { noun, verb });
			program.Execute();

			if (program.GetOpCodeAt(0) == Solution)
			{
				return 100 * noun + verb;
			}
		}
	}

	std::cerr << "Unable to find a result for Problem 2" << std::endl;
	return 0;
}