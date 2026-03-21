#include "renderer.hpp"
#include "camera.hpp"
#include "intersection.hpp"
#include "world.hpp"
#include "computations.hpp"

Canvas Renderer::Render(const Camera &camera, const World &world)
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

Color Renderer::ColorAt(const World &world, const Ray &ray)
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
    Computations comps = PrepareComputations(hit, ray, world);
    return ShadeHit(world, comps);
}

std::vector<Intersection> Renderer::IntersectWorld(const World &world, const Ray &ray)
{
    std::vector<Intersection> intersections;
    for (ObjectId objectId = 0; objectId < world.GetObjects().size(); ++objectId)
    {
        const Sphere &object = world.GetObject(objectId);
        std::vector<float> objectIntersections = Intersect(object, ray);
        for (const float &distance : objectIntersections)
        {
            Intersection intersection(distance, objectId);
            intersections.push_back(intersection);
        }
    }
    // sort intersections by t value
    std::sort(intersections.begin(), intersections.end(), [](const Intersection &a, const Intersection &b)
              { return a.GetT() < b.GetT(); });
    return intersections;
}

Color Renderer::ShadeHit(const World &world, const Computations &comps)
{
    // TODO: Handle multiple lights and shadows
    return Lighting(world.GetObject(comps.objectId).GetMaterial(), world.GetLight(0), comps.point, comps.eyeVector, comps.normalVector);
}

Color Renderer::Lighting(const Material &material, const Light &light, const Tuple &position, const Tuple &eyeVector, const Tuple &normalVector)
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

World Renderer::DefaultWorld()
{
    World w;
    Light light(Point(-10.f, 10.f, -10.f), Color(1.f, 1.f, 1.f));
    w.AddLight(light);

    Sphere s1("s1");
    s1.GetMutableMaterial().SetColor(Color(0.8f, 1.f, 0.6f));
    s1.GetMutableMaterial().SetDiffuse(0.7f);
    s1.GetMutableMaterial().SetSpecular(0.2f);
    w.AddObject(s1);

    Sphere s2("s2");
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

    return comps;
}

std::vector<float> Renderer::Intersect(const Sphere &sphere, const Ray &ray)
{
    Ray transformedRay = ray * sphere.GetTransform().GetInverse();

    // For a sphere centered at the origin with radius 1, the intersection can be calculated using the quadratic formula.
    // The coefficients of the quadratic equation are derived from substituting the ray equation into the sphere equation.
    Tuple sphereToRay = transformedRay.GetOrigin() - Point(0.f, 0.f, 0.f);   // Since the sphere is at the origin, we can use the ray's origin directly
    float a = transformedRay.GetDirection() | transformedRay.GetDirection(); // Dot product of direction with itself
    float b = 2.f * (transformedRay.GetDirection() | sphereToRay);           // 2 times the dot product of direction and origin
    float c = (sphereToRay | sphereToRay) - 1.f;                             // Dot product of origin with itself minus radius squared

    float discriminant = b * b - 4.f * a * c;

    std::vector<float> intersections;
    if (discriminant < 0.f)
    {
        // No intersections
        return intersections;
    }
    else
    {
        float sqrtDiscriminant = std::sqrt(discriminant);
        float t1 = (-b - sqrtDiscriminant) / (2.f * a);
        float t2 = (-b + sqrtDiscriminant) / (2.f * a);
        intersections.push_back(t1);
        intersections.push_back(t2);
        return intersections;
    }
}