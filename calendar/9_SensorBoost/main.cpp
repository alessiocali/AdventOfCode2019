#include <SensorBoostSolver.h>

#include <CommonHelpers.h>

int main(int argc, char** argv)
{
	SolveProblemAndDisplay<SensorBoostSolver>(SimpleGetInputFileFromArgs(argc, argv));
}