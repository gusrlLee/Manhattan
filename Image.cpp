#include "Image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

Image::Image(int width, int height) : m_Width(width), m_Height(height)
{
    m_Data = new Color[m_Width * m_Height];

    if (!m_Data)
    {
        std::clog << "Failed to create image" << std::endl;
    }
}

Image::~Image()
{
    delete[] m_Data;
}

void Image::Plot(int x, int y, const Color &color)
{
    m_Data[m_Width * y + x] = color;
}

void Image::Download(const char *fileName)
{
    int index = 0;
    uint8_t *pixels = new uint8_t[m_Width * m_Height * 3];

    for (int y = 0; y < m_Height; ++y)
    {
        for (int x = 0; x < m_Width; ++x)
        {
            Color fPixel = m_Data[m_Width * y + x];

            float r = fPixel.r();
            float g = fPixel.g();
            float b = fPixel.b();

            int ir = int(255.999 * r);
            int ig = int(255.999 * g);
            int ib = int(255.999 * b);

            pixels[index++] = ir;
            pixels[index++] = ig;
            pixels[index++] = ib;
        }
    }

    stbi_write_png(fileName, m_Width, m_Height, 3, pixels, m_Width * 3);
}

float Image::LinearToGamma(float linearComponent)
{
    if (linearComponent > 0)
        return std::sqrt(linearComponent);
    return 0;
}
