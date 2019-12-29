#include <SpaceImageFormatSolver.h>

#include <CommonHelpers.h>

int main(int argc, char** argv)
{
	SolveProblemAndDisplay<SpaceImageFormatSolver>(SpaceImageInput(25, 6, SimpleGetInputFileFromArgs(argc, argv)));
}