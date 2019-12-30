#include <1202ProgramAlarmSolver.h>

#include <IntcodeProgram.h>

std::uint32_t _1202ProgramAlarmSolver::SolveProblemA() const
{
	IntCodeComputer program(m_ProgramFilename);
	program.SetNounAndVerb({ 12, 2 });
	if (!program.IsValid())
	{
		std::cerr << "Error: couldn't process input file " << m_ProgramFilename << std::endl;
		return 0;
	}

	program.Execute();
	return program.GetValueAt(0).convert_to<std::uint32_t>();
}

std::uint32_t _1202ProgramAlarmSolver::SolveProblemB() const
{
	constexpr std::uint32_t Solution = 19690720;

	// Brute force time!
	for (std::uint32_t noun = 0; noun < 100; noun++)
	{
		for (std::uint32_t verb = 0; verb < 100; verb++)
		{
			IntCodeComputer program(m_ProgramFilename);
			program.SetNounAndVerb({ noun, verb });
			program.Execute();

			if (program.GetValueAt(0) == Solution)
			{
				return 100 * noun + verb;
			}
		}
	}

	std::cerr << "Unable to find a result for Problem 2" << std::endl;
	return 0;
}