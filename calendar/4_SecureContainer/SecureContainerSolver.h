#pragma once

#include <ProblemSolver.h>

#include <CommonDefines.h>

#include <deque>

class SecureContainerSolver : public ProblemSolver<std::string, uint, uint>
{
public:
	void Init(std::string& input) override;
	uint SolveProblemA() const override { return CountValidPasswords([](uint value) { return IsValidPassword(value); }); }
	uint SolveProblemB() const override { return CountValidPasswords([](uint value) { return IsValidRestrictedPassword(value); }); };

private:
	uint m_Min, m_Max;

	static std::deque<uint> DecomposeInDigits(uint value);
	static bool IsValidPassword(uint val);
	static bool IsValidRestrictedPassword(uint val);

	template<typename Validator>
	uint CountValidPasswords(Validator validator) const
	{
		// Brute forcing seems like a reasonable option rather than trying to compute only valid values
		// Of course this would work poorly with bigger ranges

		// Yeah yeah, decomposing digits everytime into a deque is slow as hell, but it still executes in 
		// reasonable time. At least on my PC (<6s).

		uint count = 0;
		for (uint value = m_Min; value <= m_Max; value++)
		{
			if (validator(value))
			{
				count++;
			}
		}
		return count;
	}
};