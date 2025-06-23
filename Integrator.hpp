#ifndef __MANHATTAN_CORE_INTEGRATOR_HPP__
#define __MANHATTAN_CORE_INTEGRATOR_HPP__

#include "Ray.hpp"
#include "Camera.hpp"
#include "Hittable.hpp"
#include "Image.hpp"
#include "Interval.hpp"
#include "RNG.hpp"

const int kTileSizeX = 8;
const int kTileSizeY = 8;

class Integrator
{
public:
    // Setting function
    void SetIntegratorName(std::string& name) { m_Name = name; }
    void SetCamera(std::shared_ptr<Camera> pCam) { m_CameraPtr = pCam; }
    void SetScene(std::shared_ptr<Hittable> pScene) { m_ScenePtr = pScene; }
    void SetRenderImageName(std::string name) { m_RenderImageName = name;  }
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
    Color Traversal(const Ray &r, int depth)
    {
        if (depth <= 0) return Color(0, 0, 0);

        Payload payload;
        if (m_ScenePtr->IsHit(r, Interval(0.001, kInf), payload))
        {
            Ray scattered;
            Color attenuation;
            if (payload.m_MatPtr->Scatter(r, payload, attenuation, scattered))
                return attenuation * Traversal(scattered, depth - 1);
            
            return Color(0, 0, 0);
        }

        Vector3 unitDir = Normalize(r.Direction());
        auto a = 0.5 * (unitDir.y() + 1.0);
        return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
    }

    void Render()
    {
#pragma omp parallel for schedule(dynamic)
        for (int tile = 0; tile < ((m_RenderImageWidth * m_RenderImageHeight) / (kTileSizeX * kTileSizeY)); tile++)
        {
            int x = tile % (m_RenderImageWidth / kTileSizeX);
            int y = tile / (m_RenderImageWidth / kTileSizeY);

            for (int v = 0; v < kTileSizeY; v++)
            {
                for (int u = 0; u < kTileSizeX; u++) 
                {
                    int i = kTileSizeX * x + u;
                    int j = kTileSizeY * y + v;

                    Color output = Color(0, 0, 0);
                    for (int sample = 0; sample < m_SamplePerPixel; sample++)
                    {
                        Ray r = m_CameraPtr->GenRay(i, j);
                        output += Traversal(r, m_MaxDepth);
                    }
                    m_RenderImage->Plot(i, j, output * m_SampleScale);
                }
            }
        }
#if 0 
        for (int j = 0; j < m_RenderImageHeight; j++)
        {
            for (int i = 0; i < m_RenderImageWidth; i++)
            {
                Color output = Color(0, 0, 0);
                for (int sample = 0; sample < m_SamplePerPixel; sample++)
                {
                    Ray r = m_CameraPtr->GenRay(i, j);
                    output += Traversal(r, m_MaxDepth);
                }
                m_RenderImage->Plot(i, j, output * m_SampleScale);
            }
        }
#endif
    }

    void Download() { m_RenderImage->Download(m_RenderImageName.c_str()); }

private:
    std::string m_Name;
    std::string m_RenderImageName;
    int m_RenderImageWidth, m_RenderImageHeight;
    std::shared_ptr<Image> m_RenderImage;
    std::shared_ptr<Camera> m_CameraPtr;
    std::shared_ptr<Hittable> m_ScenePtr;

    int m_SamplePerPixel;
    float m_SampleScale;
    int m_MaxDepth;
};

#endif