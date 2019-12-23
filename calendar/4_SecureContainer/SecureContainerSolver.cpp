#include <SecureContainerSolver.h>

#include <regex>

void SecureContainerSolver::Init(std::string& input)
{
	constexpr const char* inputPattern = "([1-9][0-9]*)\\-([1-9][0-9]*)";

	std::regex inputRegex(inputPattern);
	std::smatch match;
	if (std::regex_search(input, match, inputRegex) && match.size() > 2)
	{
		m_Min = static_cast<uint>(std::stoi(match.str(1)));
		m_Max = static_cast<uint>(std::stoi(match.str(2)));
	}
}

std::deque<uint> SecureContainerSolver::DecomposeInDigits(uint value)
{
	std::deque<uint> digits;
	while (value > 0)
	{
		digits.push_front(value % 10);
		value = value / 10;
	}
	return digits;
}

bool SecureContainerSolver::IsValidPassword(uint val)
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

bool SecureContainerSolver::IsValidRestrictedPassword(uint val)
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