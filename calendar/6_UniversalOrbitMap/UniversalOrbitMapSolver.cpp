#include <UniversalOrbitMapSolver.h>

#include <regex>
#include <fstream>
#include <stack>
#include <numeric>
#include <cassert>
#include <queue>

void UniversalOrbitMapSolver::Init(std::string& inputFileName)
{
	constexpr const char* OrbitPattern = "(.{3})\\)(.{3})";

	std::ifstream inputFile(inputFileName);
	if (!inputFile.is_open())
	{
		std::cerr << "Cannot oper file " << inputFileName << std::endl;
		return;
	}

	std::regex orbitRegex(OrbitPattern);
	std::smatch match;
	std::string inputLine;
	while (inputFile >> inputLine)
	{
		if (std::regex_search(inputLine, match, orbitRegex) && match.size() > 2)
		{
			AddOrbitPair(match.str(1), match.str(2));
		}
	}
}

void UniversalOrbitMapSolver::AddOrbitPair(std::string orbitCenterTag, std::string objectOrbitingTag)
{
	auto getOrbitalObjectAndAddIfMissing = [this](std::string& objectTag)
	{
		if (m_AllOrbitalObjects.count(objectTag) == 0)
		{
			m_AllOrbitalObjects[objectTag] = std::make_shared<OrbitalObject>(objectTag);
		}

		return m_AllOrbitalObjects[objectTag];
	};

	std::shared_ptr<OrbitalObject>& orbitCenter = getOrbitalObjectAndAddIfMissing(orbitCenterTag);
	std::shared_ptr<OrbitalObject>& objectOrbiting = getOrbitalObjectAndAddIfMissing(objectOrbitingTag);

	orbitCenter->AddOrbitingObject(objectOrbiting);
	
	if (objectOrbiting->GetDirectOrbitCenter() != nullptr)
	{
		std::cerr	<< "Error: " << objectOrbiting->GetName() 
					<< " already orbits around " << objectOrbiting->GetDirectOrbitCenter()->GetName() 
					<< " against desired " << orbitCenter->GetName()
					<< std::endl;
	}

	objectOrbiting->SetDirectOrbitCenter(orbitCenter);
}

uint UniversalOrbitMapSolver::SolveProblemA() const
{
	return std::accumulate(m_AllOrbitalObjects.cbegin(), m_AllOrbitalObjects.cend(), 0,
	[](uint count, const auto& orbitalObject)
	{
		return count + GetDirectAndIndirectOrbitsCount(*orbitalObject.second);
	});
}

uint UniversalOrbitMapSolver::SolveProblemB() const
{
	const std::string YOUTag = "YOU";
	const std::string SANTag = "SAN";

	assert(m_AllOrbitalObjects.count(YOUTag) > 0);
	assert(m_AllOrbitalObjects.count(SANTag) > 0);

	std::shared_ptr<OrbitalObject> startObject = m_AllOrbitalObjects.at(YOUTag)->GetDirectOrbitCenter();
	std::shared_ptr<OrbitalObject> goalObject = m_AllOrbitalObjects.at(SANTag)->GetDirectOrbitCenter();
	
	assert(startObject);
	assert(goalObject);

	// Classic Dijkstra implementation
	using NodeCostPair = std::pair<std::shared_ptr<OrbitalObject>, uint>;
	auto nodeCostComparator = [](const NodeCostPair& pair1, const NodeCostPair& pair2)
	{
		return pair1.second < pair2.second;
	};
	using NodeCostPriorityQueue = std::priority_queue<NodeCostPair, std::vector<NodeCostPair>, decltype(nodeCostComparator)>;

	std::unordered_set<std::shared_ptr<OrbitalObject>> exploredObjects;
	NodeCostPriorityQueue nodesToExplore(nodeCostComparator);

	auto addObjectIfNotExplored = [&nodesToExplore, &exploredObjects]
	(uint currentCost, const std::shared_ptr<OrbitalObject>& object)
	{
		if (exploredObjects.count(object) == 0)
		{
			// This will create useless reordering if, for example,
			// we add a node already in queue but with a lower cost.

			// To avoid that we'd need to search for and update said
			// node in the queue, but I'm not sure if/how to do that.
			// (without an expensive linear search, that is)
			
			// So for now I'll just go with the duplicates.
			// This doesn't impact the algorithm correctness at all.
			nodesToExplore.push(std::make_pair(object, currentCost + 1));
		}
	};

	nodesToExplore.push(std::make_pair(startObject, 0));
	while (!nodesToExplore.empty())
	{
		NodeCostPair currentNode = nodesToExplore.top();
		nodesToExplore.pop();

		for (const auto& objectOrbiting : currentNode.first->GetObjectsOrbitingAroundMe())
		{
			if (objectOrbiting == goalObject)
			{
				return currentNode.second + 1;
			}

			addObjectIfNotExplored(currentNode.second, objectOrbiting);
		}

		if (currentNode.first->GetDirectOrbitCenter())
		{
			if (currentNode.first->GetDirectOrbitCenter() == goalObject)
			{
				return currentNode.second + 1;
			}

			addObjectIfNotExplored(currentNode.second, currentNode.first->GetDirectOrbitCenter());
		}

		exploredObjects.insert(currentNode.first);
	}

	std::cerr << "Could not find target node" << std::endl;
	return 0;
}

uint UniversalOrbitMapSolver::GetDirectAndIndirectOrbitsCount(const OrbitalObject& orbitalObject)
{
	const OrbitalObject* current = &orbitalObject;
	uint count = 0;
	while (current->GetDirectOrbitCenter())
	{
		count++;
		current = current->GetDirectOrbitCenter().get();
	}
	return count;
}