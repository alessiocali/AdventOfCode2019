#include <AmplificationCircuitSolver.h>

#include <iostream>
#include <sstream>
#include <algorithm>
#include <cassert>

uint AmplificationCircuitSolver::SolveProblemA() const
{
	PermutationGenerator<uint> phaseGenerator({ 0, 1, 2, 3, 4 });
	return InternalSolve(LoadAmplifiersFromFile(), phaseGenerator);
}

uint AmplificationCircuitSolver::SolveProblemB() const
{
	std::vector<IntCodeComputer>& amplifiers = LoadAmplifiersFromFile();

	for (std::size_t i = 0; i < numberOfAmplifiers; i++)
	{
		amplifiers[i].SetPauseOnOutput(true);
	}

	PermutationGenerator<uint> phaseGenerator({ 5, 6, 7, 8, 9 });
	return InternalSolve(amplifiers, phaseGenerator);
}

std::vector<IntCodeComputer> AmplificationCircuitSolver::LoadAmplifiersFromFile() const
{
	std::vector<IntCodeComputer> amplifiers;

	for (std::size_t i = 0; i < numberOfAmplifiers; i++)
	{
		IntCodeComputer& amplifier = amplifiers.emplace_back(m_InputFileName);
		if (!amplifier.IsValid())
		{
			std::cerr << "Invalid input file " << m_InputFileName << std::endl;
			break;
		}
	}

	return amplifiers;
}

uint AmplificationCircuitSolver::InternalSolve(std::vector<IntCodeComputer>& amplifiers, PermutationGenerator<uint>& phaseGenerator) const
{
	assert(phaseGenerator.GetCurrentPermutation().size() == amplifiers.size());

	int currentInput = 0;
	int maxOutput = 0;

	const IntCodeComputer& lastAmplifier = *(amplifiers.end() - 1);
	do
	{
		for (std::size_t i = 0; i < amplifiers.size(); i++)
		{
			amplifiers[i].Reset();
			amplifiers[i].FeedInput(phaseGenerator.GetCurrentPermutation()[i]);
		}

		currentInput = 0;

		for (std::size_t i = 0; !lastAmplifier.IsHalted(); i = (i + 1) % amplifiers.size())
		{
			amplifiers[i].FeedInput(currentInput);
			amplifiers[i].Execute();
			amplifiers[i].GetOutput(currentInput);
		}

		int& amplifiersOutput = currentInput;
		maxOutput = std::max(maxOutput, amplifiersOutput);
	} while (phaseGenerator.ComputeNextPermutation());

	return maxOutput;
}