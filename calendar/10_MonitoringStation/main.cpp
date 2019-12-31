#include <MonitoringStationSolver.h>

#include <CommonHelpers.h>

int main(int argc, char** argv)
{
	SolveProblemAndDisplay<MonitoringStationSolver>(SimpleGetInputFileFromArgs(argc, argv));
}