#include "RNG.hpp"

std::uniform_real_distribution<float> RNG::distribution = std::uniform_real_distribution<float>(0.0, 1.0);
std::mt19937 RNG::generator;