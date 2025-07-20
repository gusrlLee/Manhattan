#include <iostream>
#include "Metal/Metal.hpp"

int main() {
    MTL::Device *device  = MTL::CreateSystemDefaultDevice();
    device->release();
    return 0;
}
