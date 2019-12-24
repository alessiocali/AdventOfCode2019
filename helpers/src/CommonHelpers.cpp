#include <CommonHelpers.h>

#include <iostream>

#include <boost/program_options.hpp>

namespace bpo = boost::program_options;

std::string SimpleGetInputFileFromArgs(int argc, char** argv)
{
	constexpr const char* AD_Input = "input,i";
	constexpr const char* AN_Input = "input";

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
		return "";
	}
	else
	{
		return varMap[AN_Input].as<std::string>();
	}
}