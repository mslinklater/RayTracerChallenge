#pragma once
#include "canvas.hpp"
#include "enums.hpp"
#include "intersection.hpp"

class Camera;
class World;
class Ray;
class Light;
class Material;
class Sphere;
class Shape;
struct Tuple;
struct Computations;

class Renderer
{
  public:
    constexpr static int kDefaultRemaining = 10;

    static Canvas Render(const Camera &camera, const World &world);
    static Color ColorAt(const World &world, const Ray &ray, int remaining = 10);
    static std::vector<Intersection> IntersectWorld(const World &world, const Ray &ray);
    static Color ReflectedColor(World &world, const Computations &comps, int remaining);
    static Color ShadeHit(const World &world, const Computations &comps, int remaining);
    static Color Lighting(const Material &material, const Shape &object, const Light &light, const Tuple &position,
                          const Tuple &eyeVector, const Tuple &normalVector, EInShadow inShadow);
    static World DefaultWorld();
    static std::vector<Intersection> Intersections(std::initializer_list<Intersection> list);
    static Intersection GetClosestIntersection(const std::vector<Intersection> &intersections);
    static Computations PrepareComputations(const Intersection &intersection, const Ray &ray, const World &world);
    static EInShadow IsShadowed(const World &world, const Tuple &point);

  private:
    Renderer() = default; // Prevent instantiation of the Renderer class
};
