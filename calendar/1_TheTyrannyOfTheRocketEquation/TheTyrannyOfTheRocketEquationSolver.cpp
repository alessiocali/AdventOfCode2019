#include <TheTyrannyOfTheRocketEquationSolver.h>

#include <cassert>
#include <fstream>
#include <numeric>

void TheTyrannyOfTheRocketEquationSolver::Init(std::string& input)
{
    m_Problem.clear();

    std::ifstream inputStream(input);
	std::uint32_t temp;
	while (inputStream >> temp)
	{
		m_Problem.push_back(temp);
	}
}

std::uint32_t TheTyrannyOfTheRocketEquationSolver::GetFuel(std::uint32_t mass)
{
    assert((mass / 3) > 1, "Negative fuel expected, value will underflow! ");
    return (mass / 3) - 2;
}

std::uint32_t TheTyrannyOfTheRocketEquationSolver::GetFuelRecursive(std::uint32_t mass)
{
	std::uint32_t fraction = mass / 3;
	if (fraction <= 2) { return 0; }
	std::uint32_t fuel = fraction - 2;
	return fuel + GetFuelRecursive(fuel);
}

std::uint32_t TheTyrannyOfTheRocketEquationSolver::SolveProblemA() const
{
    return std::accumulate(m_Problem.cbegin(), m_Problem.cend(), 0,
	[](const std::uint32_t sum, const std::uint32_t val) 
	{
		return sum + GetFuel(val);
	});
}

std::uint32_t TheTyrannyOfTheRocketEquationSolver::SolveProblemB() const
{
	return std::accumulate(m_Problem.cbegin(), m_Problem.cend(), 0,
	[](const std::uint32_t sum, const std::uint32_t val)
	{
		return sum + GetFuelRecursive(val);
	});
}