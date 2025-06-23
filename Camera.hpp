#ifndef __MANHATTAN_CORE_CAMERA_HPP__
#define __MANHATTAN_CORE_CAMERA_HPP__
#include "Vector3.hpp"
#include "Ray.hpp"
#include "RNG.hpp"
#include "Manhattan.hpp"

class Camera
{
public:
    Camera(int width, int height, Point3 position) : m_Width(width), m_Height(height), m_Position(position) { SetUp(); }
    ~Camera() = default;
    
    void Init() { SetUp(); }

    float m_Fov = 90;
    Vector3 m_From = Vector3(0, 0, 0);
    Vector3 m_At = Vector3(0, 0, -1);
    Vector3 m_Up = Vector3(0, 1, 0);

    float m_DefocusAngle = 0;
    float m_FocusDistance = 10;

    Ray GenRay(int x, int y)
    {
        auto offset = SampleInSquare();
        Point3 pxSample = m_Px00 + ((x + offset.x()) * m_Pxdu) + ((y + offset.y()) * m_Pxdv);
        auto rayOrigin = (m_DefocusAngle <= 0) ? m_Position : DefocusDiskSample();
        Vector3 rDir = pxSample - rayOrigin;

        Ray ray(rayOrigin, rDir);
        return ray;
    }

private:
    int m_Width;       // Rendered image width in pixel count
    int m_Height;      // Rendered image height
    
    Point3 m_Position; // Camera center
    Point3 m_Px00;     // Location of pixel 0, 0
    Vector3 m_Pxdu;    // Offset to pixel to the right
    Vector3 m_Pxdv;    // Offset to pixel below
    Vector3 m_U, m_V, m_W; // basis vector

    Vector3 m_DefocusDistU;
    Vector3 m_DefocusDistV;

    Vector3 SampleInSquare() const
    {
        return Vector3(RNG::Get1D() - 0.5f, RNG::Get1D() - 0.5f, 0);
    }


    Point3 DefocusDiskSample() const {
        // Returns a random point in the camera defocus disk.
        auto p = RandomInUnitDisk();
        return m_Position + (p[0] * m_DefocusDistU) + (p[1] * m_DefocusDistV);
    }
    void SetUp()
    {
        m_Position = m_From;
        auto theta = DegreesToRadians(m_Fov);
        auto h = std::tan(theta / 2);

        // Determine viewport dimensions.
        auto vHeight = 2 * h * m_FocusDistance;
        auto vWidth = vHeight * (float(m_Width) / m_Height);

        m_W = Normalize(m_From - m_At);
        m_U = Normalize(Cross(m_Up, m_W));
        m_V = Cross(m_W, m_U);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        auto vPortU = vWidth * m_U;
        auto vPortV = vHeight * -m_V;

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        m_Pxdu = vPortU / m_Width;
        m_Pxdv = vPortV / m_Height;

        // Calculate the location of the upper left pixel.
        auto upperLeft = m_Position - (m_FocusDistance * m_W) - vPortU / 2 - vPortV / 2;
        m_Px00 = upperLeft + 0.5f * (m_Pxdu + m_Pxdv);

        auto defocusRadius = m_FocusDistance * std::tan(DegreesToRadians(m_DefocusAngle / 2));
        m_DefocusDistU = m_U * defocusRadius;
        m_DefocusDistV = m_V * defocusRadius;
    }
};

#endif