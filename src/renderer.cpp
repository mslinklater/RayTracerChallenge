#include "renderer.hpp"
#include "camera.hpp"
#include "color.hpp"
#include "computations.hpp"
#include "intersection.hpp"
#include "maths.hpp"
#include "pattern.hpp"
#include "shapes/sphere.hpp"
#include "world.hpp"
#include <algorithm>
#include <atomic>
#include <cassert>
#include <cmath>
#include <thread>
#include <vector>

Canvas Renderer::Render(const Camera &camera, const World &world)
{
    assert(camera.GetHSize() > 0);
    assert(camera.GetVSize() > 0);
    Canvas canvas(camera.GetHSize(), camera.GetVSize());

    const int totalRows = camera.GetVSize();
    if (totalRows <= 0)
    {
        return canvas;
    }

    const unsigned int hardwareThreads = std::thread::hardware_concurrency();
    const unsigned int maxThreads = hardwareThreads == 0 ? 1u : hardwareThreads;
    const unsigned int threadCount = std::min(maxThreads, static_cast<unsigned int>(totalRows));

    // Used to store the row number being worked on by the worker lambda
    std::atomic<int> nextRowAtomic{0};
    std::vector<std::thread> workers;
    workers.reserve(threadCount);

    // Each worker thread will loop and continue to take unprocessed rows until the row count is reached
    auto renderRow = [&]() {
        while (true)
        {
            const int y = nextRowAtomic.fetch_add(1);
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
        }
    };

    // Create the worker threads
    for (unsigned int i = 0; i < threadCount; ++i)
    {
        workers.emplace_back(renderRow);
    }

    // Wait for all worker threads to complete
    for (std::thread &worker : workers)
    {
        worker.join();
    }

    return canvas;
}

Color Renderer::ColorAt(const World &world, const Ray &ray, int remaining)
{
    assert(remaining >= 0);
    assert(ray.IsValid());
    const IntersectionVector intersections = IntersectWorld(world, ray);
    if (intersections.empty())
    {
        return kBackgroundColor; // Return cyan if no intersections
    }
    const Intersection hit = GetClosestIntersection(intersections);
    if (hit.GetObjectId() == kInvalidObjectId)
    {
        return Color(0.f, 0.f, 0.f); // Return black if all intersections are behind the ray
    }
    Computations comps = PrepareComputations(hit, ray, world, &intersections);
    return ShadeHit(world, comps, remaining);
}

IntersectionVector Renderer::IntersectWorld(const World &world, const Ray &ray)
{
    assert(ray.IsValid());
    std::vector<Intersection> intersections;
    intersections.reserve(world.GetObjects().size() * 2);

    for (const ShapeUniquePtr &object : world.GetObjects())
    {
        const std::vector<float> objectIntersections = object->Intersect(ray);

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

Color Renderer::ReflectedColor(const World &world, const Computations &comps, int remaining)
{
    assert(remaining >= 0);
    assert(comps.IsValid());
    return ReflectedColor(world, world.GetObject(comps.objectId).GetMaterial(), comps, remaining);
}

Color Renderer::ReflectedColor(const World &world, const Material &material, const Computations &comps, int remaining)
{
    assert(remaining >= 0);
    assert(material.IsValid());
    assert(comps.IsValid());
    if (remaining <= 0)
        return kColorBlack;

    if (material.GetReflective() == 0.f)
    {
        return Color(0.f, 0.f, 0.f); // No reflection contribution if the material is not reflective
    }
    Ray reflectRay(comps.overPoint, comps.reflectv);
    const Color c = Renderer::ColorAt(world, reflectRay, --remaining);
    return c * material.GetReflective();
}

Color Renderer::RefractedColor(const World &world, const Computations &comps, int remaining)
{
    assert(remaining >= 0);
    assert(comps.IsValid());
    return RefractedColor(world, world.GetObject(comps.objectId).GetMaterial(), comps, remaining);
}

Color Renderer::RefractedColor(const World &world, const Material &material, const Computations &comps, int remaining)
{
    assert(remaining >= 0);
    assert(material.IsValid());
    assert(comps.IsValid());
    if (remaining <= 0)
        return kColorBlack;

    if (material.GetTransparency() == 0.f)
    {
        return Color(0.f, 0.f, 0.f); // No reflection contribution if the material is not reflective
    }

    float nRatio = comps.n1 / comps.n2;
    float cosI = comps.eyeVector | comps.normalVector;
    float sin2T = nRatio * nRatio * (1.f - cosI * cosI);
    if (sin2T > 1.f)
    {
        return kColorBlack; // Total internal reflection, no refraction contribution
    }

    float cosT = std::sqrt(1.f - sin2T);
    Tuple direction = comps.normalVector * (nRatio * cosI - cosT) - comps.eyeVector * nRatio;
    Ray refractRay(comps.underPoint, direction);
    Color c = Renderer::ColorAt(world, refractRay, --remaining) * material.GetTransparency();

    return c;
}

Color Renderer::ShadeHit(const World &world, const Computations &comps, int remaining)
{
    assert(remaining >= 0);
    assert(comps.IsValid());
    const Shape &object = world.GetObject(comps.objectId);
    const Material &material = object.GetMaterial();
    Color surface = kColorBlack;

    for (const Light &light : world.GetLights())
    {
        const EInShadow inShadow = IsShadowed(world, comps.overPoint, light);
        surface =
            surface + Lighting(material, object, light, comps.point, comps.eyeVector, comps.normalVector, inShadow);
    }

    Color reflected = ReflectedColor(world, material, comps, remaining);
    Color refracted = RefractedColor(world, material, comps, remaining);

    if (material.GetReflective() > 0.f && material.GetTransparency() > 0.f)
    {
        float reflectance = Schlick(comps);
        reflected = reflected * reflectance;
        refracted = refracted * (1.f - reflectance);
    }
    return surface + reflected + refracted;
}

Color Renderer::Lighting(const Material &material, const Shape &object, const Light &light, const Tuple &position,
                         const Tuple &eyeVector, const Tuple &normalVector, EInShadow inShadow)
{
    assert(material.IsValid());
    assert(light.IsValid());
    assert(position.IsValid());
    assert(eyeVector.IsValid());
    assert(normalVector.IsValid());

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

IntersectionVector Renderer::Intersections(std::initializer_list<Intersection> list)
{
    for (const Intersection &intersection : list)
    {
        assert(std::isfinite(intersection.GetT()));
    }
    IntersectionVector intersections(list);
    std::sort(intersections.begin(), intersections.end(),
              [](const Intersection &lhs, const Intersection &rhs) { return lhs.GetT() < rhs.GetT(); });
    return intersections;
}

Intersection Renderer::GetClosestIntersection(const IntersectionVector &intersections)
{
    for (const Intersection &intersection : intersections)
    {
        assert(std::isfinite(intersection.GetT()));
    }
    const auto it = std::lower_bound(intersections.begin(), intersections.end(), 0.f,
                                     [](const Intersection &intersection, float t) { return intersection.GetT() < t; });
    if (it == intersections.end())
    {
        return Intersection(0.f, kInvalidObjectId);
    }
    return *it;
}

Computations Renderer::PrepareComputations(const Intersection &intersection, const Ray &ray, const World &world,
                                           const IntersectionVector *intersectionVec)
{
    assert(intersection.GetObjectId() != kInvalidObjectId);
    assert(std::isfinite(intersection.GetT()));
    assert(ray.IsValid());
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

    comps.reflectv = ray.GetDirection().Reflect(comps.normalVector);
    comps.overPoint = comps.point + comps.normalVector * kEpsilon * 2.f;
    comps.underPoint = comps.point - comps.normalVector * kEpsilon * 2.f;

    // now work out n1 and n2
    if (intersectionVec == nullptr || intersectionVec->empty())
    {
        comps.n1 = 1.f;
        comps.n2 = 1.f;
    }
    else
    {
        std::vector<ObjectId> containers;

        // go through the intersecions setting the prev (n1) and next (n2) refractive indices
        for (auto &i : *intersectionVec)
        {
            // first, set n1
            if (i == intersection)
            {
                if (containers.empty())
                {
                    comps.n1 = 1.f;
                }
                else
                {
                    comps.n1 = world.GetObject(containers.back()).GetMaterial().GetRefractiveIndex();
                }
            }

            // find out if containers already contains this intersection
            // if to does, this must be an exit, so remove it.
            // if it doesn't, than add the intersection
            auto it = std::find(containers.begin(), containers.end(), i.GetObjectId());
            if (it != containers.end())
            {
                containers.erase(it);
            }
            else
            {
                containers.push_back(i.GetObjectId());
            }

            // now set n2
            if (i == intersection)
            {
                if (containers.empty())
                {
                    comps.n2 = 1.f;
                }
                else
                {
                    comps.n2 = world.GetObject(containers.back()).GetMaterial().GetRefractiveIndex();
                }
                break;
            }
        }
    }
    return comps;
}

EInShadow Renderer::IsShadowed(const World &world, const Tuple &point)
{
    assert(point.IsValid());
    if (world.GetLights().empty())
    {
        return EInShadow::No;
    }
    return IsShadowed(world, point, world.GetLight(0));
}

EInShadow Renderer::IsShadowed(const World &world, const Tuple &point, const Light &light)
{
    assert(point.IsValid());
    assert(light.IsValid());
    Tuple lightVector = light.position - point;
    float distanceToLight = lightVector.Magnitude();
    Ray shadowRay(point, lightVector.Normalize());

    const IntersectionVector intersections = IntersectWorld(world, shadowRay);
    const Intersection hit = GetClosestIntersection(intersections);
    if (hit.GetObjectId() != kInvalidObjectId && hit.GetT() < distanceToLight)
    {
        return EInShadow::Yes; // There is an object between the point and the light
    }
    return EInShadow::No; // No object is blocking the light
}

float Renderer::Schlick(const Computations &comps)
{
    assert(comps.IsValid());
    float cos = comps.eyeVector | comps.normalVector;
    if (comps.n1 > comps.n2)
    {
        float n = comps.n1 / comps.n2;
        float sin2T = n * n * (1.f - cos * cos);
        if (sin2T > 1.f)
        {
            return 1.f; // Total internal reflection
        }
        float cosT = std::sqrt(1.f - sin2T);
        cos = cosT;
    }
    float r0 = std::pow((comps.n1 - comps.n2) / (comps.n1 + comps.n2), 2.f);
    return r0 + (1.f - r0) * std::pow(1.f - cos, 5.f);
}
