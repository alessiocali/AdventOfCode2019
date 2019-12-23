#include <SecureContainerSolver.h>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

constexpr const char* AN_Input = "input";
constexpr const char* AD_Input = "input,i";

namespace bpo = boost::program_options;

int main(int argc, char** argv)
{
	bpo::options_description optionsDescription("Allowed options");
	optionsDescription.add_options()
		(AD_Input, bpo::value<std::string>(), "Problem input");

	bpo::variables_map varMap;
	bpo::store(bpo::parse_command_line(argc, argv, optionsDescription), varMap);
	bpo::notify(varMap);

	if (!varMap.count(AN_Input))
	{
		std::cerr << "Missing input argument" << std::endl;
		std::cout << optionsDescription << std::endl;
		return 1;
	}

	SolveProblemAndDisplay<SecureContainerSolver>(varMap[AN_Input].as<std::string>());
}