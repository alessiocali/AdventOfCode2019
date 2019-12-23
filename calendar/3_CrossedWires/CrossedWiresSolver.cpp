#include <CrossedWiresSolver.h>

#include <SimpleControllableView.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

#include <SFML/Graphics/CircleShape.hpp>

void CrossedWiresSolver::Init(std::string& inputFilename)
{
	m_Problem.clear();

	std::ifstream inputStream(inputFilename);
	if (!inputStream.is_open())
	{
		std::cerr << "Unable to open file " << inputFilename << std::endl;
		return;
	}

	std::string inputLine;
	while (std::getline(inputStream, inputLine))
	{
		m_Problem.push_back(ParseInputLine(inputLine));
	}
}

CrossedWiresSolver::Wire CrossedWiresSolver::ParseInputLine(const std::string& inputLine)
{
	Wire parsedWire;
	std::regex wireTurnRegex(WireTurnPattern);
	std::smatch match;

	parsedWire.emplace_back(Point{ 0, 0 });
	char direction;
	uint amount;

	std::stringstream inputLineStream(inputLine);
	std::string element;
	while (std::getline(inputLineStream, element, ','))
	{
		if (std::regex_search(element, match, wireTurnRegex) && match.size() > 2)
		{
			direction = match.str(1)[0];
			amount = static_cast<uint>(std::stoi(match.str(2)));
			AddPointToWire(parsedWire, direction, amount);
		}
	}

	return parsedWire;
}

void CrossedWiresSolver::AddPointToWire(OUT Wire& wire, char direction, uint amount)
{
	const Point& previous = wire.back();
	Point next = { previous.x, previous.y };
	switch (direction)
	{
	case 'U':
		next.y += amount;
		break;
	case 'L':
		next.x -= amount;
		break;
	case 'R':
		next.x += amount;
		break;
	case 'D':
		next.y -= amount;
		break;
	default:
		std::cerr << "Unhandled direction '" << direction << "'" << std::endl;
		return;
	}
	wire.push_back(next);
}

bool CrossedWiresSolver::DoSegmentsIntersect(const Segment& s1, const Segment& s2, OUT Point& intersection)
{
	if (s1.IsHorizontal() && s2.IsHorizontal())
	{
		// Do not allow overlapping wires
		return false;
	}

	if (s1.IsVertical() && s2.IsVertical())
	{
		// Ditto
		return false;
	}

	bool overlap = false;
	auto testSegment = [](const Segment& horizontal, const Segment& vertical, OUT Point& intersection)
	{
		float hy = horizontal.pt1.y;
		float vx = vertical.pt1.x;

		float vy1 = vertical.pt1.y;
		float vy2 = vertical.pt2.y;

		float hx1 = horizontal.pt1.x;
		float hx2 = horizontal.pt2.x;

		intersection = { vx, hy };

		return
			hy >= std::min(vy1, vy2) && hy <= std::max(vy1, vy2) &&
			vx >= std::min(hx1, hx2) && vx <= std::max(hx1, hx2);
	};

	if (s1.IsHorizontal())
	{
		return testSegment(s1, s2, intersection);
	}
	else
	{
		return testSegment(s2, s1, intersection);
	}
}

void CrossedWiresSolver::ComputeWiresIntersections(const Wire& w1, const Wire& w2, OUT IntersectionSet& intersections)
{
	if (w1.size() <= 1 || w2.size() <= 1)
	{
		std::cerr << "No intersections in single-point wires" << std::endl;
		return;
	}

	Point intersection;
	for (auto it1 = w1.cbegin() + 1; it1 != w1.cend(); it1++)
	{
		Segment s1 = { *(it1 - 1), *it1 };
		for (auto it2 = w2.cbegin() + 1; it2 != w2.cend(); it2++)
		{
			Segment s2 = { *(it2 - 1), *it2 };
			if (DoSegmentsIntersect(s1, s2, intersection))
			{
				intersections.insert(intersection);
			}
		}
	}
}

CrossedWiresSolver::IntersectionSet CrossedWiresSolver::ComputeIntersections(const Problem& problem)
{
	IntersectionSet intersections;
	for (auto it1 = problem.cbegin(); it1 != problem.cend(); it1++)
	{
		for (auto it2 = it1 + 1; it2 != problem.cend(); it2++)
		{
			ComputeWiresIntersections(*it1, *it2, intersections);
		}
	}
	return intersections;
}

CrossedWiresSolver::IntersectionCosts CrossedWiresSolver::ComputeIntersectionCosts(const Wire& wire, const IntersectionSet& intersections)
{
	IntersectionCosts costs;
	uint steps = 0;
	for (auto it = wire.cbegin(); it != wire.cend() - 1; it++)
	{
		const Point& current = *it;
		const Point& next = *(it + 1);

		Point point = current;
		while (point != next)
		{
			steps++;

			if (point.x < next.x) { point.x++; }
			else if (point.x > next.x) { point.x--; }
			if (point.y < next.y) { point.y++; }
			else if (point.y > next.y) { point.y--; }

			if (intersections.count(point) > 0 && costs.count(point) == 0)
			{
				costs[point] = steps;
			}
		}
	}
	return costs;
}

void CrossedWiresSolver::AppendWireCosts(OUT IntersectionCosts& cumulativeCosts, const IntersectionCosts& wireCosts)
{
	for (const auto& pair : wireCosts)
	{
		const Point& point = pair.first;
		const uint& cost = pair.second;
		uint currentCost = cumulativeCosts.count(point) > 0 ? cumulativeCosts[point] : 0;
		cumulativeCosts[point] = currentCost + cost;
	}
}

uint CrossedWiresSolver::SolveProblemA() const
{
	IntersectionSet intersections = ComputeIntersections(m_Problem);
	auto getManhattanDistance = [](const Point& point)
	{
		return std::abs(point.x) + std::abs(point.y);
	};

	auto minIntersection = std::min_element(intersections.cbegin(), intersections.cend(),
	[&getManhattanDistance](const Point& p1, const Point& p2)
	{
		return getManhattanDistance(p1) < getManhattanDistance(p2);
	});

	if (minIntersection != intersections.cend())
	{
		return static_cast<uint>(getManhattanDistance(*minIntersection));
	}

	return 0;
}

uint CrossedWiresSolver::SolveProblemB() const
{
	IntersectionSet intersections = ComputeIntersections(m_Problem);

	IntersectionCosts cumulativeCosts;
	for (const Wire& wire : m_Problem)
	{
		IntersectionCosts wireCosts = ComputeIntersectionCosts(wire, intersections);
		AppendWireCosts(cumulativeCosts, wireCosts);
	}

	const auto minIntersection = std::min_element(cumulativeCosts.cbegin(), cumulativeCosts.cend(), [](const auto& pair1, const auto& pair2)
		{
			return pair1.second < pair2.second;
		});

	return minIntersection != cumulativeCosts.cend() ? minIntersection->second : 0;
}

void CrossedWiresSolver::DebugDrawWire(const Wire& wire, sf::RenderWindow& renderWindow, sf::Color color)
{
	constexpr float thickness = 1.f;

	for (auto it = wire.cbegin(); it != wire.cend() - 1; it++)
	{
		const Point& pt1 = *it;
		const Point& pt2 = *(it + 1);

		sf::Vertex vxLine[] =
		{
			{ sf::Vector2f(pt1.x, pt1.y), color },
			{ sf::Vector2f(pt2.x, pt2.y), color }
		};

		renderWindow.draw(vxLine, 2, sf::Lines);
	}
}

void CrossedWiresSolver::DebugDrawIntersection(const Point& intersection, sf::RenderWindow& renderWindow)
{
	static constexpr float radius = 1.f;
	sf::CircleShape circle;
	circle.setFillColor(sf::Color::Green);
	circle.setRadius(radius);
	circle.setPosition(intersection.x, intersection.y);
	circle.setOrigin(radius, radius);
	renderWindow.draw(circle);
}

void CrossedWiresSolver::DebugDrawProblem(sf::RenderWindow& renderWindow, const Problem& problem)
{
	static const sf::Color colors[] = { sf::Color::Blue, sf::Color::Red, sf::Color::Yellow };
	uint i = 0;
	for (const Wire& wire : problem)
	{
		DebugDrawWire(wire, renderWindow, colors[i++ % 3]);
	}

	IntersectionSet intersections = ComputeIntersections(problem);
	for (const Point& intersection : intersections)
	{
		DebugDrawIntersection(intersection, renderWindow);
	}
}

void CrossedWiresSolver::DebugDisplay() const
{
	SimpleControllableView simpleView(sf::VideoMode(1920, 1080), "Crossed Wires", SimpleControllableView::SpeedParameters{ 0.1f, 2.f });
	simpleView.RunUntilClosed([this](auto& renderWindow)
	{
		DebugDrawProblem(renderWindow, m_Problem);
	});
}