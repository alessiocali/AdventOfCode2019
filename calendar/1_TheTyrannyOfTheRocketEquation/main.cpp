#include <TheTyrannyOfTheRocketEquationSolver.h>

int main(int argc, char** argv)
{
	if (argc <= 1)
	{
		std::cout << "Usage: " << argv[0] << " <input_file>" << std::endl;
		return 0;
	}

	SolveProblemAndDisplay<TheTyrannyOfTheRocketEquationSolver, std::string>(argv[1]);
}