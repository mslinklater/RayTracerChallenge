#pragma once

#include "sphere.hpp"
#include <string>
#include <vector>

extern int ToPPMChannel(float value);
extern std::vector<std::string> ReadLinesFromFile(const std::string &filename);
extern Sphere GlassSphere(const std::string &name);
