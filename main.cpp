#include <iostream>

#include "Metal/Metal.hpp"
#include "QuartzCore/CAMetalLayer.hpp"

#include "SDL3/SDL.h"
#include "SDL3/SDL_metal.h"

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "Failed to SDL initialization!" << std::endl;
        return -1;
    }
    
    SDL_Window* pWindow = SDL_CreateWindow("Hello World", 640, 480, SDL_WINDOW_RESIZABLE | SDL_WINDOW_METAL);
    if (!pWindow) {
        std::cerr << "Failed to create window!" << std::endl;
        SDL_Quit();
        return -1;
    }
    
    MTL::Device* device = MTL::CreateSystemDefaultDevice();
    SDL_MetalView view = SDL_Metal_CreateView(pWindow);

    CA::MetalLayer* layer = static_cast<CA::MetalLayer*>(SDL_Metal_GetLayer(view));

    layer->setPixelFormat(MTL::PixelFormatBGRA8Unorm_sRGB);
    layer->setDevice(device);
    


    
    std::cerr << device->name()->utf8String() << std::endl;

    layer->release();
    device->release();
    SDL_Metal_DestroyView(view);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return 0;
}
