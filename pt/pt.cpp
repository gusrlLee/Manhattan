#include "pt.hpp"

std::vector<Sphere> g_World;

bool Trace(const Ray &ray)
{
    bool anyHit = false;
    for (int idx = 0; idx < g_World.size(); idx++)
    {
        if (g_World[idx].Intersect(ray))
        {
            anyHit = true;
        }
    }

    return anyHit;
}

vec3 Radiance(const Ray &ray, int maxDepth)
{
    vec3 color = vec3(0, 0, 0);
    if (Trace(ray))
        return vec3(1, 1, 0);

    vec3 unitDir = normalize(ray.d);
    float t = 0.5f * (unitDir.y + 1.0f);                               // y: -1~1 -> 0~1
    return (1.0f - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0); // sky gradient
}

int main()
{
    // World setting  Sphere(position,             radius, material, color);
    g_World.push_back(Sphere(vec3(0.0f, 0, -1), 0.5f, eDIFFUSE, vec3(0.8f, 0.3f, 0.3f)));
    g_World.push_back(Sphere{vec3(0.0f, -100.5f, -1.0f), 100.0f, eDIFFUSE, vec3(0.8f, 0.8f, 0.0)});

    // Image setting
    int w = 800, h = 600;
    float aspect = (float)w / (float)h;
    vec3 *img = new vec3[w * h];

    // Camera setting
    Camera cam;
    cam.p = vec3(0, 0, 0);
    cam.fov = 90.0f;
    cam.fLen = 1.0f;

    // Renderer setting
    int spp = 1;
    int maxDepth = 10;

    // MAIN RENDERING
    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {
            for (int s = 0; s < spp; s++)
            {
                // normalized camera plane coordinate
                float u = ((2.0f * ((i + 0.5f) / float(w)) - 1.0f) * tan(cam.fov / 2.0f * M_PI / 180.0f) * aspect);
                float v = (1.0f - 2.0f * ((j + 0.5f) / float(h))) * tan(cam.fov / 2.0f * M_PI / 180.0f);

                // Genenralization Ray 
                Ray ray;
                ray.o = cam.p;
                ray.d = normalize(vec3(u, v, -cam.fLen) - cam.p);
                ray.tmin= 1e30f;

                img[j * w + i] += Radiance(ray, maxDepth);
            }
            img[j * w + i] /= (float)spp;
        }
    }

    // Image download
    FILE *f = fopen("output.ppm", "wb+");
    fprintf(f, "P3\n%d %d\n255\n", w, h);
    for (int j = 0; j < h; j++)
        for (int i = 0; i < w; i++)
            fprintf(f, "%d %d %d ", ToInt(img[i + j * w].r), ToInt(img[i + j * w].g), ToInt(img[i + j * w].b));
    fclose(f);

    delete[] img;
    return 0;
}