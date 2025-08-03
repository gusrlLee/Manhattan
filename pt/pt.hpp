#ifndef __PATH_TRACER_HPP__
#define __PATH_TRACER_HPP__

#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <cmath>

static std::uniform_real_distribution<float> distribution(0.0, 1.0);
static std::mt19937 generator;

using namespace glm;

enum Material
{
    eDIFFUSE,
    eSPECULAR
};

struct Ray
{
    vec3 o;
    vec3 d;
    float tmin;
};

struct Camera
{
    vec3 p;     // position
    float fLen; // focal length
    float fov;  // field of view
};

struct Sphere
{
    vec3 c;
    float r;
    Material mat;
    vec3 albedo;

    bool Intersect(const Ray &ray)
    {
#if 1
        vec3 oc = c - ray.o;
        float a = dot(ray.d, ray.d);
        float b = -2.0f * dot(ray.d, oc);
        float c = dot(oc, oc) - r * r;
        float det = b * b - 4 * a * c;
        if (det < 0 ) return false; // MISS 

        float sqd = sqrt(det);
        float root = (-b - sqd) / (2.0f * a);
        if (root < 0 || root > 1e30f)
        {
            root = (-b + sqd) / (2.0f * a);
            if (root < 0 || root > 1e30f)
                return false;
        }

        return true;
#else
        // vec3 oc = ray.o - c;
        // float a = dot(ray.d, ray.d);
        // float b = 2 * dot(oc, ray.d);
        // float c = dot(oc, oc) - r * r;
        // float det = b * b - 4 * a * c;
        // if (det < 0) return false;
        // else return true;
        vec3 oc = ray.o - c;
        float a = dot(ray.d, ray.d);
        float half_b = dot(oc, ray.d);
        float c = dot(oc, oc) - r * r;
        float disc = half_b * half_b - a * c;
        if (disc < 0)
            return false;
        float sqd = sqrt(disc);

        // 먼저 작은 root 시도
        float root = (-half_b - sqd) / a;
        if (root < 0 || root > 1e30f)
        {
            // 못 쓰면 큰 root 사용
            root = (-half_b + sqd) / a;
            if (root < 0 || root > 1e30f)
                return false;
        }

        return true;

#endif
    }
};

// Util function
inline float Clamp(float x) { return x < 0 ? 0 : (x > 1 ? 1 : x); }
inline int ToInt(float v) { return int(pow(Clamp(v), 1 / 2.2) * 255 + .5); }

inline float Sample1D() { return Clamp(distribution(generator)); }
inline vec2 Sample2D() { return vec2(Sample1D(), Sample1D()); }

#endif // __PATH_TRACER_HPP__