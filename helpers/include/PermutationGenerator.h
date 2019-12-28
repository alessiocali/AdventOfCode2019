#pragma once

#include <vector>
#include <algorithm>
#include <type_traits>

/***********************************************************************************************

 Yeah I know std::get_next_permutation exists, but I wanted to implement the SJT algorithm...

 https://www.geeksforgeeks.org/johnson-trotter-algorithm/

************************************************************************************************/

template<typename T, typename Container = std::vector<T>, std::enable_if_t<!std::is_reference_v<Container>, bool> = 0>
class PermutationGenerator
{
public:
	PermutationGenerator(Container container)
		: m_ContainedData(container)
	{
		for (std::size_t i = 1; i <= m_ContainedData.size(); i++)
		{
			m_PermutationIndices.push_back(-i);
		}
	}

	bool ComputeNextPermutation()
	{
		IndicesContainer::iterator maxMovableIt = m_PermutationIndices.end();
		auto maxMovable = 0;
		for (auto it = m_PermutationIndices.begin(); it != m_PermutationIndices.end(); it++)
		{
			if (IsMovable(it) && std::abs(*it) > maxMovable)
			{
				maxMovableIt = it;
				maxMovable = *it;
			}
		}

		if (maxMovableIt == m_PermutationIndices.end())
		{
			return false;
		}

		const int swapDirection = maxMovable > 0 ? 1 : -1;
		std::iter_swap(maxMovableIt, maxMovableIt + swapDirection);

		std::size_t movableIdx = std::distance(m_PermutationIndices.begin(), maxMovableIt);
		auto movableInCollectionIt = m_ContainedData.begin() + movableIdx;
		std::iter_swap(movableInCollectionIt, movableInCollectionIt + swapDirection);

		for (auto& permutationIdx : m_PermutationIndices)
		{
			if (std::abs(permutationIdx) > std::abs(maxMovable))
			{
				permutationIdx *= -1;
			}
		}

		return true;
	}

	inline Container& GetCurrentPermutation() { return m_ContainedData; }

private:
	using IndicesContainer = std::vector<int>;

	Container m_ContainedData;
	IndicesContainer m_PermutationIndices;

	bool IsMovable(IndicesContainer::iterator it)
	{
		int swapDirection = *it > 0 ? 1 : -1;
		IndicesContainer::iterator next = (it == m_PermutationIndices.begin() && swapDirection < 0) ? m_PermutationIndices.end() : it + swapDirection;

		if (next == m_PermutationIndices.end())
		{
			return false;
		}

		return std::abs(*it) > std::abs(*next);
	}
};