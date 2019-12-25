#include <ProblemSolver.h>

#include <CommonDefines.h>

#include <unordered_set>
#include <unordered_map>

class UniversalOrbitMapSolver : public ProblemSolver<std::string, uint, uint>
{
public:
	void Init(std::string& inputFileName) override;
	uint SolveProblemA() const override;
	uint SolveProblemB() const override;

private:
	class OrbitalObject
	{
	public:
		explicit OrbitalObject(std::string name)
			: m_Name(std::move(name))
		{ }

		inline void AddOrbitingObject(std::shared_ptr<OrbitalObject> object) { m_ObjectsOrbitingAroundMe.insert(object); }
		inline void SetDirectOrbitCenter(std::shared_ptr<OrbitalObject> center) { m_DirectOrbitCenter = center; }

		inline const std::string& GetName() const { return m_Name; }
		inline const std::unordered_set<std::shared_ptr<OrbitalObject>>& GetObjectsOrbitingAroundMe() const { return m_ObjectsOrbitingAroundMe; }
		inline const std::shared_ptr<OrbitalObject>& GetDirectOrbitCenter() const { return m_DirectOrbitCenter; }

	private:
		std::string m_Name;
		std::unordered_set<std::shared_ptr<OrbitalObject>> m_ObjectsOrbitingAroundMe;
		std::shared_ptr<OrbitalObject> m_DirectOrbitCenter;
	};

	static uint GetDirectAndIndirectOrbitsCount(const OrbitalObject& orbitalObject);

	void AddOrbitPair(std::string orbitCenter, std::string orbitingObject);

	std::unordered_map<std::string, std::shared_ptr<OrbitalObject>> m_AllOrbitalObjects;
};