#include <1202ProgramAlarmSolver.h>

int main(int argc, char** argv)
{
	if (argc <= 1)
	{
		std::cout << "Usage: " << argv[0] << " <input-file> " << std::endl;
		return 0;
	}

	SolveProblemAndDisplay<_1202ProgramAlarmSolver, std::string>(argv[1]);
}