#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <TheTyrannyOfTheRocketEquationSolver.h>
#include <1202ProgramAlarmSolver.h>
#include <CrossedWiresSolver.h>
#include <SecureContainerSolver.h>
#include <SunnyWithAChanceOfAsteroidsSolver.h>
#include <UniversalOrbitMapSolver.h>
#include <AmplificationCircuitSolver.h>
#include <SpaceImageFormatSolver.h>

template<typename Solver, typename InputType, typename SolutionAType, typename SolutionBType>
void ValidateProblem(InputType input, const SolutionAType& solutionA, const SolutionBType& solutionB)
{
	Solver solver;
	solver.Init(input);

	REQUIRE(solver.SolveProblemA() == solutionA);
	REQUIRE(solver.SolveProblemB() == solutionB);
}

TEST_CASE("TheTyrannyOfTheRocketEquation")
{
	constexpr const char* input = "inputs/Tyranny_Input.txt";
	ValidateProblem<TheTyrannyOfTheRocketEquationSolver, std::string>(input, 3231941, 4845049);
}

TEST_CASE("1202ProgramAlarm")
{
	constexpr const char* input = "inputs/1202_Input.txt";
	ValidateProblem<_1202ProgramAlarmSolver, std::string>(input, 3267740, 7870);
}

TEST_CASE("CrossedWires")
{
	constexpr const char* input = "inputs/Wires_Input.txt";
	ValidateProblem<CrossedWiresSolver, std::string>(input, 860, 9238);
}

TEST_CASE("SecureContainer")
{
	constexpr const char* input = "307237-769058";
	ValidateProblem<SecureContainerSolver, std::string>(input, 889, 589);
}

TEST_CASE("SunnyWithAChanceOfAsteroids")
{
	constexpr const char* input = "inputs/Sunny_Input.txt";
	ValidateProblem<SunnyWithAChanceOfAsteroidsSolver, std::string>(input, 12896948, 7704130);
}

TEST_CASE("UniversalOrbitMap")
{
	constexpr const char* input = "inputs/Orbit_Input.txt";
	ValidateProblem<UniversalOrbitMapSolver, std::string>(input, 154386, 346);
}

TEST_CASE("AmplificationCircuit")
{
	constexpr const char* input = "inputs/Amplification_Input.txt";
	ValidateProblem<AmplificationCircuitSolver, std::string>(input, 273814, 34579864);
}

TEST_CASE("SpaceImageFormat")
{
	constexpr const char* inputFile = "inputs/SpaceImage_Input.txt";

	constexpr const char* problemBSolution =
		"\n\n"
		"###  #     ##  #  # #### \n"
		"#  # #    #  # # #  #    \n"
		"#  # #    #  # ##   ###  \n"
		"###  #    #### # #  #    \n"
		"# #  #    #  # # #  #    \n"
		"#  # #### #  # #  # #    \n"
		"\n";

	SpaceImageInput input(25, 6, std::string(inputFile));
	ValidateProblem<SpaceImageFormatSolver>(input, 1485, std::string(problemBSolution));
}