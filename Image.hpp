#ifndef __MANHATTAN_CORE_IMAGE_HPP__
#define __MANHATTAN_CORE_IMAGE_HPP__

#include "Color.hpp"

class Image {
    public:
        Image(int width, int height);
        ~Image();

        void Plot(int x, int y, const Color &color);
        void Download(const char* filename);

    private:
        float LinearToGamma(float linearComponent);
        int m_Width, m_Height;
        Color *m_Data;
};

#endif
