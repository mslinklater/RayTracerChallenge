#include "renderer.hpp"
#include "camera.hpp"
#include "computations.hpp"
#include "intersection.hpp"
#include "maths.hpp"
#include "sphere.hpp"
#include "world.hpp"
#include <algorithm>
#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

Canvas Renderer::Render(const Camera &camera, const World &world)
{
    Canvas canvas(camera.GetHSize(), camera.GetVSize());

    const int totalRows = camera.GetVSize();
    if (totalRows <= 0)
    {
        return canvas;
    }

    const unsigned int hardwareThreads = std::thread::hardware_concurrency();
    const unsigned int maxThreads = hardwareThreads == 0 ? 1u : hardwareThreads;
    const unsigned int threadCount = std::min(maxThreads, static_cast<unsigned int>(totalRows));

    std::atomic<int> nextRow{0};
    std::atomic<int> completedRows{0};
    std::mutex outputMutex;
    std::vector<std::thread> workers;
    workers.reserve(threadCount);

    std::cout << "Rendering using " << threadCount << " threads for " << totalRows << " rows\n" << std::flush;

    auto renderRows = [&]() {
        while (true)
        {
            const int y = nextRow.fetch_add(1);
            if (y >= totalRows)
            {
                return;
            }

            for (int x = 0; x < camera.GetHSize(); ++x)
            {
                Ray ray = camera.RayForPixel(x, y);
                Color color = ColorAt(world, ray);
                canvas.WritePixel(x, y, color);
            }

            const int done = completedRows.fetch_add(1) + 1;
            std::lock_guard<std::mutex> lock(outputMutex);
            std::cout << "Rendering row " << done << " of " << totalRows << "\r" << std::flush;
        }
    };

    for (unsigned int i = 0; i < threadCount; ++i)
    {
        workers.emplace_back(renderRows);
    }

    for (std::thread &worker : workers)
    {
        worker.join();
    }

    return canvas;
}

Color Renderer::ColorAt(const World &world, const Ray &ray)
{
    std::vector<Intersection> intersections = IntersectWorld(world, ray);
    if (intersections.empty())
    {
        return kBackgroundColor; // Return cyan if no intersections
    }
    // find the first intersection with a positive t value
    auto it = std::find_if(intersections.begin(), intersections.end(),
                           [](const Intersection &intersection) { return intersection.GetT() >= 0.f; });
    if (it == intersections.end())
    {
        return Color(0.f, 0.f, 0.f); // Return black if all intersections are behind the ray
    }
    const Intersection &hit = *it;
    Computations comps = PrepareComputations(hit, ray, world);
    return ShadeHit(world, comps);
}

std::vector<Intersection> Renderer::IntersectWorld(const World &world, const Ray &ray)
{
    std::vector<Intersection> intersections;

    for (const ShapePtr &object : world.GetObjects())
    {
        //   const Shape &object = world.GetObject(objectId);
        std::vector<float> objectIntersections = object->Intersect(ray);

        for (const float &distance : objectIntersections)
        {
            Intersection intersection(distance, object->GetWorldObjectId());
            intersections.push_back(intersection);
        }
    }

    // sort intersections by t value
    std::sort(intersections.begin(), intersections.end(),
              [](const Intersection &a, const Intersection &b) { return a.GetT() < b.GetT(); });
    return intersections;
}

Color Renderer::ShadeHit(const World &world, const Computations &comps)
{
    EInShadow inShadow = IsShadowed(world, comps.overPoint);

    return Lighting(world.GetObject(comps.objectId).GetMaterial(), world.GetObject(comps.objectId), world.GetLight(0),
                    comps.point, comps.eyeVector, comps.normalVector, inShadow);
}

Color Renderer::Lighting(const Material &material, const Shape &object, const Light &light, const Tuple &position,
                         const Tuple &eyeVector, const Tuple &normalVector, EInShadow inShadow)
{
    Color color;

    if (material.GetPattern() != nullptr)
    {
        color = material.GetPattern()->PatternAtShape(object, position);
    }
    else
    {
        color = material.GetColor();
    }

    Color effectiveColor = color * light.intensity;

    Tuple lightVector = (light.position - position).Normalize();

    // the three components of the Phong reflection model: ambient, diffuse, and specular
    Color ambient = effectiveColor * material.GetAmbient();

    if (inShadow == EInShadow::Yes)
    {
        return ambient; // Only ambient component contributes to the color if the point is in shadow
    }

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

World Renderer::DefaultWorld()
{
    World w;
    Light light(Point(-10.f, 10.f, -10.f), Color(1.f, 1.f, 1.f));
    w.AddLight(light);

    Sphere s1("external");
    s1.GetMutableMaterial().SetColor(Color(0.8f, 1.f, 0.6f));
    s1.GetMutableMaterial().SetDiffuse(0.7f);
    s1.GetMutableMaterial().SetSpecular(0.2f);
    w.AddObject(s1);

    Sphere s2("internal");
    s2.SetTransform(Matrix::CreateScaling(0.5f, 0.5f, 0.5f));
    w.AddObject(s2);
    return w;
}

std::vector<Intersection> Renderer::Intersections(std::initializer_list<Intersection> list)
{
    return std::vector<Intersection>(list);
}

Intersection Renderer::GetClosestIntersection(const std::vector<Intersection> &intersections)
{
    Intersection hit(0.f, kInvalidObjectId);
    for (const auto &intersection : intersections)
    {
        if (intersection.GetT() >= 0.f)
        {
            if (hit.GetObjectId() == kInvalidObjectId || intersection.GetT() < hit.GetT())
            {
                hit = intersection;
            }
        }
    }
    return hit;
}

Computations Renderer::PrepareComputations(const Intersection &intersection, const Ray &ray, const World &world)
{
    Computations comps;
    comps.t = intersection.GetT();
    comps.objectId = intersection.GetObjectId();
    comps.point = ray.PositionAt(comps.t);
    comps.eyeVector = -ray.GetDirection();
    comps.normalVector = world.GetObject(comps.objectId).NormalAt(comps.point);

    if ((comps.normalVector | comps.eyeVector) < 0.f)
    {
        comps.inside = true;
        comps.normalVector = -comps.normalVector;
    }

    comps.overPoint = comps.point + comps.normalVector * kEpsilon * 2.f;
    return comps;
}

EInShadow Renderer::IsShadowed(const World &world, const Tuple &point)
{
    const Light &light = world.GetLight(0); // Assuming only one light for now
    Tuple lightVector = light.position - point;
    float distanceToLight = lightVector.Magnitude();
    Ray shadowRay(point, lightVector.Normalize());

    std::vector<Intersection> intersections = IntersectWorld(world, shadowRay);
    auto it =
        std::find_if(intersections.begin(), intersections.end(), [distanceToLight](const Intersection &intersection) {
            return intersection.GetT() >= 0.f && intersection.GetT() < distanceToLight;
        });
    if (it != intersections.end())
    {
        return EInShadow::Yes; // There is an object between the point and the light
    }
    return EInShadow::No; // No object is blocking the light
}
