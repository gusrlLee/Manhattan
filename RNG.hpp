#ifndef __MANHATTAN_CORE_RANDOM_NUMBER_GENERATION_HPP__
#define __MANHATTAN_CORE_RANDOM_NUMBER_GENERATION_HPP__

#include <cstdlib>
#include <random>
#include <iostream>

class RNG
{
public:
    static std::uniform_real_distribution<float> distribution;
    static std::mt19937 generator;

    static float Get1D() { return distribution(generator); }
    static float Get1D(float min, float max) { return (max - min) * Get1D(); }

};

#endif