#pragma once

#include <CommonDefines.h>

#include <string>
#include <vector>

std::string SimpleGetInputFileFromArgs(int argc, char** argv);
std::vector<uint> DecomposeInDigits(uint value);