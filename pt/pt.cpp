#include "pt.hpp"

std::vector<Sphere> g_World;

bool Trace(const Ray &ray, SurfaceInteraction &si)
{
    bool anyHit = false;
    si.tMin = 1e30f;

    SurfaceInteraction tsi; // test surface interaction
    for (int idx = 0; idx < g_World.size(); idx++)
    {
        if (g_World[idx].Intersect(ray, tsi))
        {
            anyHit = true;
            if (tsi.tMin < si.tMin)
            {
                si.idx = idx;
                si.tMin = tsi.tMin;
            }
        }
    }

    return anyHit;
}

vec3 Radiance(const Ray &ray, int maxDepth)
{
    vec3 color = vec3(0, 0, 0);
    vec3 thp = vec3(1, 1, 1);
    Ray wi = ray;

    for (int depth = 0; depth < maxDepth; depth++)
    {
        SurfaceInteraction si;
        si.tMin = 1e30f;

        if (!Trace(wi, si))
            break;

        vec3 x = wi.At(si.tMin);                    // intersection point
        vec3 n = normalize(x - g_World[si.idx].c);     // normal vector of intersection surface
        vec3 nl = dot(n, wi.d) < 0 ? n : n * -1.0f; // set face normal
        vec3 f = g_World[si.idx].color;
        float p = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z;

        // if (depth > 5) {
        //     if (Sample1D() < p) 
        //     {
        //         f = f * (1 / p);
        //     } 
        //     else 
        //     {
        //         color += thp * g_World[si.idx].emit;
        //         break;
        //     }
        // }

        color += thp * g_World[si.idx].emit;
        thp *= g_World[si.idx].color;

        Ray wo;
        if (g_World[si.idx].mat == eDIFFUSE)
        {
            vec3 w = nl, u = normalize(cross(vec3(0, 1, 0), w)), v = cross(w, u);
            float theta = 2 * M_PI * Sample1D(), r2 = Sample1D(), r = sqrt(r2);
            vec3 d = u * cos(theta) * r + v * sin(theta) * r + w * sqrt(1 - r2);

            wo.o = x;
            wo.d = normalize(d);
        }

        wi = wo;
    }

    return color;
}

int main()
{
    // World setting  Sphere(radius, position, emit, color, material)
    g_World.push_back(Sphere(0.3f, vec3(0.5f, 0, -1), vec3(0, 0, 0), vec3(0.75f, 0.75f, 0.75f), eDIFFUSE));         // right ball
    g_World.push_back(Sphere(0.3f, vec3(-0.5f, 0, -1), vec3(0, 0, 0), vec3(0.75f, 0.75f, 0.75f), eDIFFUSE));        // left ball
    g_World.push_back(Sphere(100.0f, vec3(0, -100.5f, -1.0f), vec3(0, 0, 0), vec3(0.75f, 0.75f, 0.75f), eDIFFUSE)); // Ground
    g_World.push_back(Sphere(0.1f, vec3(0, 0.5f, -2), vec3(12, 12, 12), vec3(0, 0, 0), eDIFFUSE));                  // Light

    // Image setting
    int w = 800, h = 800;
    float aspect = (float)w / (float)h;
    vec3 *img = new vec3[w * h];

    // Camera setting
    Camera cam;
    cam.p = vec3(0, 0, 0);
    cam.fov = 90.0f;
    cam.fLen = 1.0f;

    // Renderer setting
    int spp = 128;
    int maxDepth = 10;

    // MAIN RENDERING
    // TODO: Multi-threading!!
    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {
            for (int s = 0; s < spp; s++)
            {
                // normalized camera plane coordinate
                float u = ((2.0f * ((i + Sample1D()) / float(w)) - 1.0f) * tan(cam.fov / 2.0f * M_PI / 180.0f) * aspect);
                float v = (1.0f - 2.0f * ((j + Sample1D()) / float(h))) * tan(cam.fov / 2.0f * M_PI / 180.0f);

                // Genenralization Ray
                Ray ray;
                ray.o = cam.p;
                ray.d = normalize(vec3(u, v, -cam.fLen) - cam.p);
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