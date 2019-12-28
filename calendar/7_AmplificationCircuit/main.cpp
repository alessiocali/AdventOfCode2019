#include <CommonHelpers.h>
#include <AmplificationCircuitSolver.h>

int main(int argc, char** argv)
{
	SolveProblemAndDisplay<AmplificationCircuitSolver>(SimpleGetInputFileFromArgs(argc, argv));
}