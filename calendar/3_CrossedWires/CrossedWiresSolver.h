#include <ProblemSolver.h>

#include <CommonDefines.h>

#include <vector>
#include <unordered_set>
#include <unordered_map>

#include <SFML/Graphics/Color.hpp>

namespace sf
{
	class RenderWindow;
}

class CrossedWiresSolver : public ProblemSolver<std::string, uint, uint>
{
public:
	struct Point
	{
		float x, y;

		bool operator==(const Point& other) const { return x == other.x && y == other.y; }
		bool operator!=(const Point& other) const { return !(*this == other); }

		std::size_t hash() const
		{
			return static_cast<std::uint32_t>(x) ^ static_cast<std::uint32_t>(y);
		}
	};

	struct Segment
	{
		const Point& pt1, pt2;

		bool IsHorizontal() const { return pt1.y == pt2.y; }
		bool IsVertical() const { return pt1.x == pt2.y; }
	};

	using Wire = std::vector<Point>;
	using Problem = std::vector<Wire>;

	void Init(std::string& inputFile) override;
	uint SolveProblemA() const override;
	uint SolveProblemB() const override;

	void DebugDisplay() const override;

private:
	using IntersectionSet = std::unordered_set<Point>;
	using IntersectionCosts = std::unordered_map<Point, unsigned int>;

	static constexpr char* WireTurnPattern = "(U|L|R|D)([1-9][0-9]*)";

	static Wire ParseInputLine(const std::string& inputLine);
	static void AddPointToWire(OUT Wire& wire, char direction, uint amount);
	static bool DoSegmentsIntersect(const Segment& s1, const Segment& s2, OUT Point& intersection);
	static void ComputeWiresIntersections(const Wire& w1, const Wire& w2, OUT IntersectionSet& intersections);
	static IntersectionSet ComputeIntersections(const Problem& problem);
	static IntersectionCosts ComputeIntersectionCosts(const Wire& wire, const IntersectionSet& intersections);
	static void AppendWireCosts(OUT IntersectionCosts& cumulativeCosts, const IntersectionCosts& wireCosts);

	static void DebugDrawWire(const Wire& wire, sf::RenderWindow& renderWindow, sf::Color color);
	static void DebugDrawIntersection(const Point& intersection, sf::RenderWindow& renderWindow);
	static void DebugDrawProblem(sf::RenderWindow& renderWindow, const Problem& problem);

	Problem m_Problem;
};

namespace std
{
	template<>
	struct hash<CrossedWiresSolver::Point>
	{
		std::size_t operator()(const CrossedWiresSolver::Point& point) const
		{
			return point.hash();
		}
	};
}