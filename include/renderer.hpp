#pragma once
#include "canvas.hpp"
#include "intersection.hpp"
#include "enums.hpp"

class Camera;
class World;
class Ray;
class Light;
class Material;
class Sphere;
struct Tuple;
struct Computations;

class Renderer
{
public:
    static Canvas Render(const Camera &camera, const World &world);
    static Color ColorAt(const World &world, const Ray &ray);
    static std::vector<Intersection> IntersectWorld(const World &world, const Ray &ray);
    static Color ShadeHit(const World &world, const Computations &comps);
    static Color Lighting(const Material &material, const Light &light, const Tuple &position, const Tuple &eyeVector, const Tuple &normalVector, EInShadow inShadow);
    static World DefaultWorld();
    static std::vector<Intersection> Intersections(std::initializer_list<Intersection> list);
    static Intersection GetClosestIntersection(const std::vector<Intersection> &intersections);
    static Computations PrepareComputations(const Intersection &intersection, const Ray &ray, const World &world);
    static std::vector<float> Intersect(const Sphere &sphere, const Ray &ray);
    static EInShadow IsShadowed(const World &world, const Tuple &point);

private:
    Renderer() = default; // Prevent instantiation of the Renderer class
};