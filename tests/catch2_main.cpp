#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <TheTyrannyOfTheRocketEquationSolver.h>
#include <1202ProgramAlarmSolver.h>
#include <CrossedWiresSolver.h>
#include <SecureContainerSolver.h>

template<typename Solver, typename InputType, typename SolutionAType, typename SolutionBType>
void ValidateProblem(InputType input, SolutionAType solutionA, SolutionBType solutionB)
{
	Solver solver;
	solver.Init(input);

	REQUIRE(solver.SolveProblemA() == solutionA);
	REQUIRE(solver.SolveProblemB() == solutionB);
}

SCENARIO("The Tyranny of the Rocket Equation")
{
	constexpr const char* input = "inputs/Tyranny_Input.txt";
	ValidateProblem<TheTyrannyOfTheRocketEquationSolver, std::string>(input, 3231941, 4845049);
}

SCENARIO("1202 Program Alarm")
{
	constexpr const char* input = "inputs/1202_Input.txt";
	ValidateProblem<_1202ProgramAlarmSolver, std::string>(input, 3267740, 7870);
}

SCENARIO("Crossed Wires")
{
	constexpr const char* input = "inputs/Wires_Input.txt";
	ValidateProblem<CrossedWiresSolver, std::string>(input, 860, 9238);
}

SCENARIO("Secure Container")
{
	constexpr const char* input = "307237-769058";
	ValidateProblem<SecureContainerSolver, std::string>(input, 889, 589);
}