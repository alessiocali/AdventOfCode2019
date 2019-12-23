#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>

std::vector<std::uint32_t> ReadInput(std::string fileName)
{
	std::vector<std::uint32_t> values;
	std::ifstream inputStream(fileName);
	std::uint32_t temp;
	while (inputStream >> temp)
	{
		values.push_back(temp);
	}
	return values;
}

std::uint32_t GetFuel(std::uint32_t mass)
{
	assert((mass / 3) > 1, "Negative fuel expected, value will underflow! ");
	return (mass / 3) - 2;
}

std::uint32_t ComputeResult_1(const std::vector<std::uint32_t>& input)
{
	return std::accumulate(input.cbegin(), input.cend(), 0,
	[](const std::uint32_t sum, const std::uint32_t val) 
	{
		return sum + GetFuel(val);
	});
}

std::uint32_t GetFuelRecursive(std::uint32_t mass)
{
	std::uint32_t fraction = mass / 3;
	if (fraction <= 2) { return 0; }
	std::uint32_t fuel = fraction - 2;
	return fuel + GetFuelRecursive(fuel);
}

std::uint32_t ComputeResult_2(const std::vector<std::uint32_t>& input)
{
	return std::accumulate(input.cbegin(), input.cend(), 0,
	[](const std::uint32_t sum, const std::uint32_t val)
	{
		return sum + GetFuelRecursive(val);
	});
}

int main(int argc, char** argv)
{
	if (argc <= 1)
	{
		std::cout << "Usage: " << argv[0] << " <input_file>" << std::endl;
		return 0;
	}

	std::vector<std::uint32_t> input = ReadInput(argv[1]);
	std::cout << "Solution of problem 1: " << ComputeResult_1(input) << std::endl;
	std::cout << "Solution of problem 2: " << ComputeResult_2(input) << std::endl;
	return 0;
}