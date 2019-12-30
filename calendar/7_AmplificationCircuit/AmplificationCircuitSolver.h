#include <ProblemSolver.h>

#include <CommonDefines.h>
#include <IntcodeProgram.h>
#include <PermutationGenerator.h>

#include <vector>

class AmplificationCircuitSolver : public ProblemSolver<std::string, uint, uint>
{
public:
	void Init(std::string& inputFileName) override { m_InputFileName = inputFileName; }
	uint SolveProblemA() const override;
	uint SolveProblemB() const override;

private:
	static constexpr uint numberOfPhases = 5;
	static constexpr std::size_t numberOfAmplifiers = 5;

	std::string m_InputFileName;

	std::vector<IntCodeComputer> LoadAmplifiersFromFile() const;
	uint InternalSolve(std::vector<IntCodeComputer>& amplifiers, PermutationGenerator<uint>& phaseGenerator) const;
};