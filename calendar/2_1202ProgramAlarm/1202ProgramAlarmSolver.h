#pragma once

#include <ProblemSolver.h>

class _1202ProgramAlarmSolver : public ProblemSolver<std::string, std::uint32_t, std::uint32_t>
{
public:
	void Init(std::string& input) override { m_ProgramFilename = input;	};
	std::uint32_t SolveProblemA() const override;
	std::uint32_t SolveProblemB() const override;

private:
	std::string m_ProgramFilename;
};