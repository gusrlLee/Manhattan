#ifndef __MANHATTAN_HPP__
#define __MANHATTAN_HPP__

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

// Constants
const float kInf = std::numeric_limits<float>::infinity();
const float kPi = (float)3.1415926535897932385;

inline float DrgreesToRadians(float degrees) {
    return degrees * kPi / 180.0f;
}

// common header
#include "Color.hpp"
#include "Ray.hpp"
#include "Vector3.hpp"

#endif