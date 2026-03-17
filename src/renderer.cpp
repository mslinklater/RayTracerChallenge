#include "renderer.hpp"
#include "camera.hpp"
#include "intersection.hpp"
#include "world.hpp"

Canvas Render(const Camera &camera, const World &world)
{
    Canvas canvas(camera.GetHSize(), camera.GetVSize());

    for (int y = 0; y < camera.GetVSize(); y++)
    {
        for (int x = 0; x < camera.GetHSize(); x++)
        {
            Ray ray = camera.RayForPixel(x, y);
            Color color = ColorAt(world, ray);
            canvas.WritePixel(x, y, color);
        }
    }

    return canvas;
}

Color ColorAt(const World &world, const Ray &ray)
{
    std::vector<Intersection> intersections = IntersectWorld(world, ray);
    if (intersections.empty())
    {
        return Color(0.f, 0.f, 0.f); // Return black if no intersections
    }
    // find the first intersection with a positive t value
    auto it = std::find_if(intersections.begin(), intersections.end(), [](const Intersection &intersection)
                           { return intersection.GetT() >= 0.f; });
    if (it == intersections.end())
    {
        return Color(0.f, 0.f, 0.f); // Return black if all intersections are behind the ray
    }
    const Intersection &hit = *it;
    Computations comps = PrepareComputations(hit, ray);
    return ShadeHit(world, comps);
}

std::vector<Intersection> IntersectWorld(const World &world, const Ray &ray)
{
    std::vector<Intersection> intersections;
    for (const auto &object : world.GetObjects())
    {
        std::vector<float> objectIntersections = Intersect(object, ray);
        for (const auto &distance : objectIntersections)
        {
            Intersection intersection(distance, &object);
            intersections.push_back(intersection);
        }
    }
    // sort intersections by t value
    std::sort(intersections.begin(), intersections.end(), [](const Intersection &a, const Intersection &b)
              { return a.GetT() < b.GetT(); });
    return intersections;
}

Color ShadeHit(const World &world, const Computations &comps)
{
    return Lighting(comps.object->GetMaterial(), world.GetLight(0), comps.point, comps.eyeVector, comps.normalVector);
}

Color Lighting(const Material &material, const Light &light, const Tuple &position, const Tuple &eyeVector, const Tuple &normalVector)
{
    Color effectiveColor = material.GetColor() * light.intensity;
    Tuple lightVector = (light.position - position).Normalize();

    // the three components of the Phong reflection model: ambient, diffuse, and specular
    Color ambient = effectiveColor * material.GetAmbient();
    Color diffuse;
    Color specular;

    float lightDotNormal = lightVector | normalVector;

    if (lightDotNormal < 0.f)
    {
        diffuse = Color(0.f, 0.f, 0.f);
        specular = Color(0.f, 0.f, 0.f);
    }
    else
    {
        diffuse = effectiveColor * material.GetDiffuse() * lightDotNormal;

        Tuple reflectVector = (-lightVector).Reflect(normalVector);
        float reflectDotEye = reflectVector | eyeVector;

        if (reflectDotEye <= 0.f)
        {
            specular = Color(0.f, 0.f, 0.f);
        }
        else
        {
            float factor = std::pow(reflectDotEye, material.GetShininess());
            specular = light.intensity * material.GetSpecular() * factor;
        }
    }

    return ambient + diffuse + specular;
}
