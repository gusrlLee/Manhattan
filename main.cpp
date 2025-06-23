#include <iostream>
#include "Image.hpp"
#include "Color.hpp"
#include "Ray.hpp"
#include "Integrator.hpp"

#include "Camera.hpp"

int main(int argc, char *argv[])
{
    std::shared_ptr<Integrator> pIntegrator = std::make_shared<Integrator>();
    int width = 720;
    int height = 480;

    // render image setting 
    std::string name = "Output.png";
    pIntegrator->SetRenderImage(width, height);
    pIntegrator->SetRenderImageName(name);

    // camera setting 
    std::shared_ptr<Camera> pCam = std::make_shared<Camera>(width, height, Point3(0, 0, 0));
    pCam->m_Fov = 20.0f;
    pCam->m_From = Point3(13, 2, 3);
    pCam->m_At = Point3(0, 0, 0);
    pCam->m_Up = Vector3(0, 1, 0);

    pCam->m_DefocusAngle = 0.6;
    pCam->m_FocusDistance = 10.0;
    pCam->Init();
    
    pIntegrator->SetCamera(pCam);

    // world setting 
    std::shared_ptr<HittableList> world = std::make_shared<HittableList>();
    auto groundMaterial = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world->Add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, groundMaterial));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = RNG::Get1D();
            Point3 center(a + 0.9 * RNG::Get1D(), 0.2, b + 0.9 * RNG::Get1D());

            if ((center - Point3(4, 0.2, 0)).Length() > 0.9) {
                std::shared_ptr<Material> sphereMaterial;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color::Random() * Color::Random();
                    sphereMaterial = std::make_shared<Lambertian>(albedo);
                    world->Add(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color::Random(0.5, 1);
                    auto fuzz = RNG::Get1D(0, 0.5);
                    sphereMaterial = std::make_shared<Metal>(albedo, fuzz);
                    world->Add(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
                }
                else {
                    // glass
                    sphereMaterial = std::make_shared<Dielectric>(1.5);
                    world->Add(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
                 }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    world->Add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world->Add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world->Add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));


#if 0
    auto matGround = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto matCenter = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto matLeft = std::make_shared<Dielectric>(1.50);
    auto matBubble = std::make_shared<Dielectric>(1.00 / 1.50);
    auto matRight = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0f);

    world->Add(std::make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, matGround));
    world->Add(std::make_shared<Sphere>(Point3(0.0, 0.0, -1.2), 0.5, matCenter));
    world->Add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, matLeft));
    world->Add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.4, matBubble));
    world->Add(std::make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, matRight));
#endif
    pIntegrator->SetScene(world);

    // number of sample per pixel setting 
    pIntegrator->SetSamplePerPixel(10);

    // ray depth 
    pIntegrator->SetRayMaxDepth(10);

    // rendering start 
    pIntegrator->Render();

    // render image download
    pIntegrator->Download();

    return 0;
}
