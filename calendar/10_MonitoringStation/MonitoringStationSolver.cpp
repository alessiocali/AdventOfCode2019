#include <MonitoringStationSolver.h>

#include <fstream>
#include <algorithm>
#include <queue>
#include <map>
#include <unordered_map>

#include <boost/math/constants/constants.hpp>

Position Position::IntNormalize() const 
{
	// Since we are dealing with integer arithmetic we can't use length
	// based normalization. Instead we divide both X and Y by their MCD

	// Your friendly neighbourhood Euclide's algorithm
	int residual = 0;
	int a = std::abs(m_X), b = std::abs(m_Y);
	while (b != 0)
	{
		residual = a % b;
		a = b;
		b = residual;
	}

	int& mcd = a;
	return { m_X / mcd, m_Y / mcd };
}

void MonitoringStationSolver::Init(std::string& inputFileName)
{
	std::ifstream inputFile(inputFileName);
	if (!inputFile.is_open())
	{
		std::cerr << "Unable to open file " << inputFileName << std::endl;
		return;
	}

	std::string inputString;
	int currentY = 0;
	while (inputFile >> inputString)
	{
		int currentX = 0;
		for (const char ch : inputString)
		{
			if (ch == '#')
			{
				m_AsteroidPositions.emplace(Position(currentX, currentY));
			}
			currentX++;
		}
		currentY++;
	}

	auto xComparator = [](const auto& pos1, const auto& pos2)
	{
		return pos1.m_X < pos2.m_X;
	};

	auto yComparator = [](const auto& pos1, const auto& pos2)
	{
		return pos1.m_Y < pos2.m_Y;
	};

	int minX = std::min_element(m_AsteroidPositions.begin(), m_AsteroidPositions.end(), xComparator)->m_X;
	int minY = std::min_element(m_AsteroidPositions.begin(), m_AsteroidPositions.end(), yComparator)->m_Y;
	int maxX = std::max_element(m_AsteroidPositions.begin(), m_AsteroidPositions.end(), xComparator)->m_X;
	int maxY = std::max_element(m_AsteroidPositions.begin(), m_AsteroidPositions.end(), yComparator)->m_Y;

	m_MinCoordinate = { minX, minY };
	m_MaxCoordinate = { maxX, maxY };
}

std::size_t MonitoringStationSolver::SolveProblemA() const
{
	return GetBestAsteroidPositionAndViewableCount().second;
}

uint MonitoringStationSolver::SolveProblemB() const
{
	const Position stationPosition = GetBestAsteroidPositionAndViewableCount().first;

	auto lineOfSightSorter = [](const Position& p1, const Position& p2)
	{
		// Priority queue shows 'larger' items first, so the furthest positions must be 'less'
		return p1.GetTaxiLength() > p2.GetTaxiLength();
	};

	// Since we use the angle only for ordering, floating point arithmetic is fine
	auto verticalAngleSorter = [](const Position& p1, const Position& p2)
	{
		// Order of arguments is intentional, remember we need 
		// the clockwise angle with the vertical axis
		auto getPositiveAtan2 = [](const Position& p)
		{
			constexpr float _2pi = 2 * boost::math::constants::pi<float>();

			float atan = std::atan2f(float(p.m_X), -float(p.m_Y));
			return atan >= 0.f ? atan : atan + _2pi;
		};

		return getPositiveAtan2(p1) < getPositiveAtan2(p2);
	};

	using LineOfSightGroup = std::priority_queue<Position, std::vector<Position>, decltype(lineOfSightSorter)>;
	using LineOfSightMap = std::map<Position, LineOfSightGroup, decltype(verticalAngleSorter)>;

	LineOfSightMap losGroupsByDirection(verticalAngleSorter);
	for (const Position& asteroidPosition : m_AsteroidPositions)
	{
		if (asteroidPosition == stationPosition)
		{
			continue;
		}

		const Position direction = (asteroidPosition - stationPosition).IntNormalize();
		if (losGroupsByDirection.count(direction) == 0)
		{
			losGroupsByDirection.emplace(direction, lineOfSightSorter);
		}

		// Asteroids are stored in local position wtr to the station
		losGroupsByDirection.at(direction).push(asteroidPosition - stationPosition);
	}

	std::size_t pewCount = 0;
	auto mapIt = losGroupsByDirection.begin();
	while (!losGroupsByDirection.empty())
	{
		LineOfSightGroup& losGroup = mapIt->second;
		const Position toVaporize = losGroup.top();

		losGroup.pop();
		pewCount++;

		if (pewCount == 200)
		{
			const Position asteroidGlobalPosition = toVaporize + stationPosition;
			return asteroidGlobalPosition.m_X * 100 + asteroidGlobalPosition.m_Y;
		}

		if (losGroup.empty())
		{
			mapIt = losGroupsByDirection.erase(mapIt);
		}
		else
		{
			mapIt++;
		}

		if (mapIt == losGroupsByDirection.end())
		{
			mapIt = losGroupsByDirection.begin();
		}
	}

	std::cerr << "Couldn't find 200th vaporized asteroid, somehow." << std::endl;
	return 0;
}

std::pair<Position, std::size_t> MonitoringStationSolver::GetBestAsteroidPositionAndViewableCount() const
{
	std::unordered_map<Position, std::unordered_set<Position>> viewableAsteroidsMap;
	for (const Position& asteroidPosition : m_AsteroidPositions)
	{
		std::unordered_set<Position>& viewableAsteroids =
			viewableAsteroidsMap.emplace(asteroidPosition, m_AsteroidPositions).first->second;

		// An asteroid can't see itself
		viewableAsteroids.erase(asteroidPosition);

		// Initially all asteroids are viewable, then we iterate 
		// each other asteroid and remove those they obstruct.
		for (const Position& otherPosition : m_AsteroidPositions)
		{
			if (otherPosition == asteroidPosition)
			{
				continue;
			}

			// Walk following the direction, and if we find other asteroids we remove them from our sight
			const Position direction = (otherPosition - asteroidPosition).IntNormalize();
			for (Position lineOfSight = otherPosition + direction; IsWithinBounds(lineOfSight); lineOfSight = lineOfSight + direction)
			{
				if (m_AsteroidPositions.count(lineOfSight))
				{
					viewableAsteroids.erase(lineOfSight);
				}
			}
		}
	}

	auto bestAsteroid = std::max_element(viewableAsteroidsMap.begin(), viewableAsteroidsMap.end(),
	[](const auto& pair1, const auto& pair2)
	{
		return pair1.second.size() < pair2.second.size();
	});

	return std::make_pair<>(bestAsteroid->first, bestAsteroid->second.size());
}

bool MonitoringStationSolver::IsWithinBounds(const Position& pos) const
{
	return	m_MinCoordinate.m_X <= pos.m_X && pos.m_X <= m_MaxCoordinate.m_X
		&&	m_MinCoordinate.m_Y <= pos.m_Y && pos.m_Y <= m_MaxCoordinate.m_Y;
}