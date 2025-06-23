#include <iostream>
#include "Image.hpp"
#include "Color.hpp"
#include "Ray.hpp"
#include "Integrator.hpp"

#include "Camera.hpp"

int main(int argc, char *argv[])
{
    std::shared_ptr<Integrator> pIntegrator = std::make_shared<Integrator>();
    int width = 500;
    int height = 500;

    // render image setting 
    std::string name = "Output.png";
    pIntegrator->SetRenderImage(width, height);
    pIntegrator->SetName(name);

    // camera setting 
    std::shared_ptr<Camera> pCam = std::make_shared<Camera>(width, height, Point3(0, 0, 0));
    pIntegrator->SetCamera(pCam);

    // world setting 
    std::shared_ptr<HittableList> world = std::make_shared<HittableList>();
    world->Add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world->Add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));
    pIntegrator->SetScene(world);
    // std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(Point3(0, 0, -1), 0.5);
    // pIntegrator->SetScene(sphere);

    // number of sample per pixel setting 
    pIntegrator->SetSamplePerPixel(4);

    // ray depth 
    pIntegrator->SetRayMaxDepth(10);

    // rendering start 
    pIntegrator->Render();

    // render image download
    pIntegrator->Download();

    return 0;
}
