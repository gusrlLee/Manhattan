#ifndef __MANHATTAN_CORE_GEOMETRY_HPP__
#define __MANHATTAN_CORE_GEOMETRY_HPP__

#include "Ray.hpp"
#include "Interval.hpp"

class Hittable
{
public:
    virtual ~Hittable() = default;
    virtual bool IsHit(const Ray &r, Interval interval, Payload &payload) const = 0;
};

class Sphere : public Hittable
{
public:
    Sphere(const Point3 &center, float radius) : m_Center(center), m_Radius(std::fmax(0, radius)) {}

    bool IsHit(const Ray &r, Interval interval, Payload &payload) const override
    {
        Vector3 oc = m_Center - r.Origin();
        auto a = r.Direction().LengthSquared();
        auto h = Dot(r.Direction(), oc);
        auto c = oc.LengthSquared() - m_Radius * m_Radius;

        auto discriminant = h * h - a * c;
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (h - sqrtd) / a;
        if (!interval.Surrounds(root))
        {
            root = (h + sqrtd) / a;
            if (!interval.Surrounds(root))
                return false;
        }

        payload.m_Tvalue = root;
        payload.m_HitPoint = r.At(payload.m_Tvalue);
        Vector3 outwardNormal = (payload.m_HitPoint - m_Center) / m_Radius;
        payload.SetFrontFace(r, outwardNormal);

        return true;
    }

private:
    Point3 m_Center;
    float m_Radius;
};

class HittableList : public Hittable
{
public:
    std::vector<std::shared_ptr<Hittable>> m_Objects;

    HittableList() {}
    HittableList(std::shared_ptr<Hittable> object) { Add(object); }

    void Clear() { m_Objects.clear(); }

    void Add(std::shared_ptr<Hittable> object)
    {
        m_Objects.push_back(object);
    }

    bool IsHit(const Ray &r, Interval interval, Payload &payload) const override
    {
        Payload tempPayload;
        bool isAnyHit = false;
        float closestSoFar = interval.m_Max;

        for (const auto &object : m_Objects)
        {
            if (object->IsHit(r, Interval(interval.m_Min, closestSoFar), tempPayload))
            {
                isAnyHit = true;
                closestSoFar = tempPayload.m_Tvalue;
                payload = tempPayload;
            }
        }

        return isAnyHit;
    }
};

#endif