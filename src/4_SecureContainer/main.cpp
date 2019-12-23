#include <iostream>
#include <algorithm>
#include <regex>
#include <deque>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

constexpr const char* AN_Input = "input";
constexpr const char* AD_Input = "input,i";

namespace bpo = boost::program_options;

using uint = unsigned int;

struct Problem
{
	uint min, max;
};

Problem ParseInput(std::string input)
{
	constexpr const char* inputPattern = "([1-9][0-9]*)\\-([1-9][0-9]*)";

	std::regex inputRegex(inputPattern);
	std::smatch match;
	if (std::regex_search(input, match, inputRegex) && match.size() > 2)
	{
		return Problem{ static_cast<uint>(std::stoi(match.str(1))), static_cast<uint>(std::stoi(match.str(2))) };
	}

	return Problem{ 0, 0 };
}

std::deque<uint> DecomposeInDigits(uint value)
{
	std::deque<uint> digits;
	while (value > 0)
	{
		digits.push_front(value % 10);
		value = value / 10;
	}
	return digits;
}

bool IsValidPassword(uint val)
{
	std::deque<uint> digits = DecomposeInDigits(val);
	
	bool hasDoubleValues = false;
	bool areDigitsIncreasing = true;
	for (auto it = digits.begin(); it != digits.end() - 1 && areDigitsIncreasing; it++)
	{
		hasDoubleValues = hasDoubleValues || *it == *(it + 1);
		areDigitsIncreasing = areDigitsIncreasing && *it <= *(it + 1);
	}
	
	return hasDoubleValues && areDigitsIncreasing;
}

bool IsValidRestrictedPassword(uint val)
{
	std::deque<uint> digits = DecomposeInDigits(val);

	bool hasStrictlyDoubleValues = false;
	bool areDigitsIncreasing = true;
	for (auto it = digits.begin(); it != digits.end() - 1 && areDigitsIncreasing; it++)
	{
		const bool previousWasDifferent = (it == digits.begin() || *it != *(it - 1));
		const bool nextIsSame = *it == *(it + 1);
		const bool nextAfterNextIsDifferent = ((it + 2) == digits.end() || *(it + 1) != *(it + 2));
		const bool isAStrictlyDoubleValue = previousWasDifferent && nextIsSame && nextAfterNextIsDifferent;

		hasStrictlyDoubleValues = hasStrictlyDoubleValues || isAStrictlyDoubleValue;
		areDigitsIncreasing = areDigitsIncreasing && *it <= *(it + 1);
	}

	return hasStrictlyDoubleValues && areDigitsIncreasing;
}

template<typename Validator>
uint CountValidPasswords(const Problem& problem, Validator validator)
{
	// Brute forcing seems like a reasonable option rather than trying to compute only valid values
	// Of course this would work poorly with bigger ranges

	// Yeah yeah, decomposing digits everytime into a deque is slow as hell, but it still executes in 
	// reasonable time. At least on my PC (<6s).

	uint count = 0;
	for (uint value = problem.min; value <= problem.max; value++)
	{
		if (validator(value))
		{
			count++;
		}
	}
	return count;
}

uint ComputeResult_1(const Problem& problem)
{
	return CountValidPasswords(problem, [](uint value) { return IsValidPassword(value); });
}

uint ComputeResult_2(const Problem& problem)
{
	return CountValidPasswords(problem, [](uint value) { return IsValidRestrictedPassword(value); });
}

int main(int argc, char** argv)
{
	bpo::options_description optionsDescription("Allowed options");
	optionsDescription.add_options()
		(AD_Input, bpo::value<std::string>(), "Problem input");

	bpo::variables_map varMap;
	bpo::store(bpo::parse_command_line(argc, argv, optionsDescription), varMap);
	bpo::notify(varMap);

	if (!varMap.count(AN_Input))
	{
		std::cerr << "Missing input argument" << std::endl;
		std::cout << optionsDescription << std::endl;
		return 1;
	}

	Problem problem = ParseInput(varMap[AN_Input].as<std::string>());
	std::cout << "Result of problem 1: " << ComputeResult_1(problem) << std::endl;
	std::cout << "Result of problem 2: " << ComputeResult_2(problem) << std::endl;
}