#pragma once
#include "canvas.hpp"
#include "intersection.hpp"

class Camera;
class World;
class Ray;
class Light;
class Material;
class Sphere;
struct Tuple;
struct Computations;

extern Canvas Render(const Camera &camera, const World &world);
extern Color ColorAt(const World &world, const Ray &ray);
extern std::vector<Intersection> IntersectWorld(const World &world, const Ray &ray);
extern Color ShadeHit(const World &world, const Computations &comps);
extern Color Lighting(const Material &material, const Light &light, const Tuple &position, const Tuple &eyeVector, const Tuple &normalVector);
extern World DefaultWorld();
extern std::vector<Intersection> Intersections(std::initializer_list<Intersection> list);
extern Intersection GetClosestIntersection(const std::vector<Intersection> &intersections);
extern Computations PrepareComputations(const Intersection &intersection, const Ray &ray, const World &world);
extern std::vector<float> Intersect(const Sphere &sphere, const Ray &ray);

class Renderer
{
public:
    static Canvas Render(const Camera &camera, const World &world);
    static Color ColorAt(const World &world, const Ray &ray);
    static std::vector<Intersection> IntersectWorld(const World &world, const Ray &ray);
    static Color ShadeHit(const World &world, const Computations &comps);
    static Color Lighting(const Material &material, const Light &light, const Tuple &position, const Tuple &eyeVector, const Tuple &normalVector);
    static World DefaultWorld();
    static std::vector<Intersection> Intersections(std::initializer_list<Intersection> list);
    static Intersection GetClosestIntersection(const std::vector<Intersection> &intersections);
    static Computations PrepareComputations(const Intersection &intersection, const Ray &ray, const World &world);
    static std::vector<float> Intersect(const Sphere &sphere, const Ray &ray);

private:
    Renderer() = default; // Prevent instantiation of the Renderer class
};