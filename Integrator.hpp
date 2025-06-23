#ifndef __MANHATTAN_CORE_INTEGRATOR_HPP__
#define __MANHATTAN_CORE_INTEGRATOR_HPP__

#include "Ray.hpp"
#include "Camera.hpp"
#include "Hittable.hpp"
#include "Image.hpp"
#include "Interval.hpp"

class Integrator
{
public:
    // Setting function
    void SetName(std::string &name) { m_Name = name; }
    void SetCamera(std::shared_ptr<Camera> pCam) { m_CameraPtr = pCam; }
    void SetScene(std::shared_ptr<Hittable> pScene) { m_ScenePtr = pScene; }
    void SetRenderImage(int width, int height)
    {
        m_RenderImageWidth = width;
        m_RenderImageHeight = height;
        m_RenderImage = std::make_shared<Image>(m_RenderImageWidth, m_RenderImageHeight);
    }
    void SetSamplePerPixel(int n)
    {
        m_SamplePerPixel = n;
        m_SampleScale = 1.0f / m_SamplePerPixel;
    }

    void SetRayMaxDepth(int n) { m_MaxDepth = n; }

    // Main rendering function
    Color Traversal(const Ray &r)
    {
        Payload payload;
        if (m_ScenePtr->IsHit(r, Interval(0.001, kInf), payload))
        {
            return 0.5f * (payload.m_Normal + Color(1, 1, 1));
        }

        Vector3 unitDir = Normalize(r.Direction());
        auto a = 0.5 * (unitDir.y() + 1.0);
        return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
    }

    void Render()
    {
        for (int j = 0; j < m_RenderImageHeight; j++)
        {
            for (int i = 0; i < m_RenderImageWidth; i++)
            {
                Color output = Color(0, 0, 0);
                for (int sample = 0; sample < m_SamplePerPixel; sample++)
                {
                    Ray r = m_CameraPtr->GenRay(i, j);
                    output += Traversal(r);
                }
                m_RenderImage->Plot(i, j, output * m_SampleScale);
            }
        }
    }

    void Download() { m_RenderImage->Download(m_Name.c_str()); }

private:
    std::string m_Name;
    int m_RenderImageWidth, m_RenderImageHeight;
    std::shared_ptr<Image> m_RenderImage;
    std::shared_ptr<Camera> m_CameraPtr;
    std::shared_ptr<Hittable> m_ScenePtr;

    int m_SamplePerPixel;
    float m_SampleScale;
    int m_MaxDepth;
};

#endif