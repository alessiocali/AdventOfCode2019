#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <TheTyrannyOfTheRocketEquationSolver.h>
#include <1202ProgramAlarmSolver.h>
#include <CrossedWiresSolver.h>
#include <SecureContainerSolver.h>
#include <SunnyWithAChanceOfAsteroidsSolver.h>

template<typename Solver, typename InputType, typename SolutionAType, typename SolutionBType>
void ValidateProblem(InputType input, SolutionAType solutionA, SolutionBType solutionB)
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