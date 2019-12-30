#pragma once

#include <ProblemSolver.h>

#include <IntcodeProgram.h>

class SensorBoostSolver : public ProblemSolver<std::string, IntCodeValue, IntCodeValue>
{
public:
	void Init(std::string& fileName) override { m_InputFileName = fileName; }
	IntCodeValue SolveProblemA() const override { return SolveWithInput(1); }
	IntCodeValue SolveProblemB() const override { return SolveWithInput(2); }

private:
	IntCodeValue SolveWithInput(IntCodeValue input) const;

	std::string m_InputFileName;
};