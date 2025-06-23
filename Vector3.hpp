#ifndef __MANHATTAN_CORE_VECTOR3_HPP__
#define __MANHATTAN_CORE_VECTOR3_HPP__

#include "RNG.hpp"
#include <iostream>

class Vector3
{
public:
    float e[3];
    
    Vector3() : e{0, 0, 0} {}
    Vector3(float e0, float e1, float e2) : e{e0, e1, e2} {}

    float x() const { return e[0]; }
    float y() const { return e[1]; }
    float z() const { return e[2]; }

    float r() const { return e[0]; }
    float g() const { return e[1]; }
    float b() const { return e[2]; }

    Vector3 operator-() const { return Vector3(-e[0], -e[1], -e[2]); }
    float operator[](int i) const { return e[i]; }
    float &operator[](int i) { return e[i]; }

    Vector3 &operator+=(const Vector3 &v)
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    Vector3 &operator*=(float t)
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    Vector3 &operator/=(float t)
    {
        return *this *= 1 / t;
    }

    float Length() const
    {
        return std::sqrt(LengthSquared());
    }

    float LengthSquared() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }
};

using Vertex3 = Vector3;
using Point3 = Vector3;

inline std::ostream &operator<<(std::ostream &out, const Vector3 &v)
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vector3 operator+(const Vector3 &u, const Vector3 &v)
{
    return Vector3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vector3 operator-(const Vector3 &u, const Vector3 &v)
{
    return Vector3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vector3 operator*(const Vector3 &u, const Vector3 &v)
{
    return Vector3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vector3 operator*(float t, const Vector3 &v)
{
    return Vector3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vector3 operator*(const Vector3 &v, float t)
{
    return t * v;
}

inline Vector3 operator/(const Vector3 &v, float t)
{
    return (1 / t) * v;
}

inline float Dot(const Vector3 &u, const Vector3 &v)
{
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline Vector3 Cross(const Vector3 &u, const Vector3 &v)
{
    return Vector3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                   u.e[2] * v.e[0] - u.e[0] * v.e[2],
                   u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vector3 Normalize(const Vector3 &v)
{
    return v / v.Length();
}



#endif
