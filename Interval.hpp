#ifndef __MANHATTAN_CORE_INTERVAL_HPP__
#define __MANHATTAN_CORE_INTERVAL_HPP__
#include "Manhattan.hpp"

class Interval
{
public:
    float m_Min, m_Max;

    Interval() : m_Min(+kInf), m_Max(-kInf) {} // Default interval is empty
    Interval(float min, float max) : m_Min(min), m_Max(max) {}
    float Size() const
    {
        return m_Max - m_Min;
    }

    bool Contains(float x) const
    {
        return m_Min <= x && x <= m_Max;
    }

    bool Surrounds(float x) const
    {
        return (m_Min < x && x < m_Max);
    }

    float Clamp(float x) const
    {
        if (x < m_Min)
            return m_Min;
        if (x > m_Max)
            return m_Max;
        return x;
    }

    static const Interval s_Empty, s_Universe;
};

#endif