#include <CrossedWiresSolver.h>

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cout << "Usage: " << argv[0] << "<input-file>" << std::endl;
		return 0;
	}

	SolveProblemAndDisplay<CrossedWiresSolver, std::string>(argv[1]);
}