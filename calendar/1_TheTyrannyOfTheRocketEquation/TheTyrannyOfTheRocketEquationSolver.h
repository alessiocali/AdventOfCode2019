#include <ProblemSolver.h>

#include <vector>

class TheTyrannyOfTheRocketEquationSolver : public ProblemSolver<std::string, std::uint32_t, std::uint32_t>
{
public:
    void Init(std::string& input) override;
    std::uint32_t SolveProblemA() const override;
    std::uint32_t SolveProblemB() const override;

private:
    using Problem = std::vector<std::uint32_t>;

    static std::uint32_t GetFuel(std::uint32_t mass);
    static std::uint32_t GetFuelRecursive(std::uint32_t mass);

    Problem m_Problem;
};