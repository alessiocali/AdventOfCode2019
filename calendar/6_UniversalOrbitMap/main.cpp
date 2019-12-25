#include <UniversalOrbitMapSolver.h>

#include <CommonHelpers.h>

int main(int argc, char** argv)
{
	SolveProblemAndDisplay<UniversalOrbitMapSolver>(SimpleGetInputFileFromArgs(argc, argv));
}