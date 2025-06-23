#ifndef __MANHATTAN_CORE_RAY_HPP__
#define __MANHATTAN_CORE_RAY_HPP__

#include "Vector3.hpp"

class Ray {
public:
    Ray() {}
    Ray(const Point3 &origin, const Vector3 &direction) : m_Origin(origin), m_Direction(direction) {}

    const Point3 &Origin() const { return m_Origin; }
    const Vector3 &Direction() const { return m_Direction; }
    Point3 At(float t) const { return m_Origin + m_Direction * t; }

private:
    Point3 m_Origin;
    Vector3 m_Direction;
};

class Payload
{
public:
    Point3 m_HitPoint;
    Vector3 m_Normal;
    float m_Tvalue;
    bool m_IsFrontFace;

    void SetFrontFace(const Ray &ray, const Vector3 &normal)
    {
        m_IsFrontFace = Dot(ray.Direction(), normal) < 0.0f;
        m_Normal = m_IsFrontFace ? normal : -normal;
    }
};

#endif
