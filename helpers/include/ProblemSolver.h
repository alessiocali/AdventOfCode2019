#pragma once

#include <iostream>

template<typename Solver, typename InputType>
void SolveProblemAndDisplay(InputType input)
{
    Solver solver;
    solver.Init(input);
    solver.OutputResult();
	solver.DebugDisplay();
}

template<typename InputType, typename SolutionAType, typename SolutionBType>
class ProblemSolver
{
public:
	virtual void Init(InputType& input) = 0;
	virtual SolutionAType SolveProblemA() const = 0;
	virtual SolutionBType SolveProblemB() const = 0;

	virtual void DebugDisplay() const { };

    void OutputResult() const
    {
		SolutionAType solutionA = SolveProblemA();
		SolutionBType solutionB = SolveProblemB();

        std::cout << "Solution of Problem A: " << solutionA << std::endl;
        std::cout << "Solution of Problem B: " << solutionB << std::endl;
    }
};