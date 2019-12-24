#pragma once

#include <ProblemSolver.h>

#include <vector>

class SunnyWithAChanceOfAsteroidsSolver : public ProblemSolver<std::string, std::uint32_t, std::uint32_t>
{
public:
	void Init(std::string& inputFilename) override { m_InputFilename = inputFilename; }
	std::uint32_t SolveProblemA() const override;
	std::uint32_t SolveProblemB() const override;

private:
	std::string m_InputFilename;

	std::uint32_t SolveWithInput(std::uint32_t input) const;
	static std::vector<int> GetOutputAsIntegers(std::istream& inputStream);
};