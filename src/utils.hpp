#pragma once

#include <cmath>
#include <vector>
#include <string>

extern bool AreEqual(float a, float b);
extern int ToPPMChannel(float value);
extern std::vector<std::string> ReadLinesFromFile(const std::string &filename);