#pragma once

#include <ProblemSolver.h>

#include <CommonDefines.h>

#include <unordered_set>

struct Position
{
public:
	Position() : Position(0, 0)
	{ }

	Position(int x, int y)
		: m_X(x), m_Y(y)
	{ }
	
	int m_X, m_Y;

	Position IntNormalize() const;

	inline int GetTaxiLength() const { return std::abs(m_X) + std::abs(m_Y); }

	inline bool operator==(const Position& other) const { return m_X == other.m_X && m_Y == other.m_Y; }
	inline std::size_t hash() const { return m_X ^ m_Y; }
};

namespace std
{
	template<>
	struct hash<Position>
	{
		std::size_t operator()(const Position& pos) const { return pos.hash(); }
	};
}

inline Position operator-(const Position& p1, const Position& p2)
{
	return Position{ p1.m_X - p2.m_X, p1.m_Y - p2.m_Y };
}

inline Position operator+(const Position& p1, const Position& p2)
{
	return Position{ p1.m_X + p2.m_X, p1.m_Y + p2.m_Y };
} 

class MonitoringStationSolver : public ProblemSolver<std::string, std::size_t, uint>
{
public:
	void Init(std::string& inputFileName) override;
	std::size_t SolveProblemA() const override;
	uint SolveProblemB() const override;

private:
	std::pair<Position, std::size_t> GetBestAsteroidPositionAndViewableCount() const;
	bool IsWithinBounds(const Position& pos) const;

	std::unordered_set<Position> m_AsteroidPositions;

	Position m_MinCoordinate;
	Position m_MaxCoordinate;
};