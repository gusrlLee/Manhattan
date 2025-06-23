#ifndef __MANHATTAN_CORE_CAMERA_HPP__
#define __MANHATTAN_CORE_CAMERA_HPP__
#include "Vector3.hpp"
#include "Ray.hpp"
#include "RNG.hpp"

class Camera
{
public:
    Camera(int width, int height, Point3 position) : m_Width(width), m_Height(height), m_Position(position) { SetUp(); }
    ~Camera() = default;

    Ray GenRay(int x, int y)
    {
        auto offset = SampleInSquare();
        Point3 pxCenter = m_Px00 + ((x + offset.x()) * m_Pxdu) + ((y + offset.y()) * m_Pxdv);
        Vector3 rDir = pxCenter - m_Position;
        Ray ray(m_Position, rDir);
        return ray;
    }

private:
    int m_Width;       // Rendered image width in pixel count
    int m_Height;      // Rendered image height
    
    Point3 m_Position; // Camera center
    Point3 m_Px00;     // Location of pixel 0, 0
    Vector3 m_Pxdu;    // Offset to pixel to the right
    Vector3 m_Pxdv;    // Offset to pixel below

    Vector3 SampleInSquare() const
    {
        return Vector3(RNG::Get1D() - 0.5f, RNG::Get1D() - 0.5f, 0);
    }

    void SetUp()
    {
        // Determine viewport dimensions.
        auto fLength = 1.0f;
        auto vHeight = 2.0f;
        auto vWidth = vHeight * (float(m_Width) / m_Height);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        auto vPortU = Vector3(vWidth, 0, 0);
        auto vPortV = Vector3(0, -vHeight, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        m_Pxdu = vPortU / m_Width;
        m_Pxdv = vPortV / m_Height;

        // Calculate the location of the upper left pixel.
        auto upperLeft = m_Position - Vector3(0, 0, fLength) - vPortU / 2 - vPortV / 2;
        m_Px00 = upperLeft + 0.5f * (m_Pxdu + m_Pxdv);
    }
};

#endif