#pragma once
#include "canvas.hpp"
#include "intersection.hpp"

class Camera;
class World;
class Ray;
class Light;

extern Canvas Render(const Camera &camera, const World &world);
extern Color ColorAt(const World &world, const Ray &ray);
extern std::vector<Intersection> IntersectWorld(const World &world, const Ray &ray);
extern Color ShadeHit(const World &world, const Computations &comps);
extern Color Lighting(const Material &material, const Light &light, const Tuple &position, const Tuple &eyeVector, const Tuple &normalVector);
extern World DefaultWorld();
extern std::vector<Intersection> Intersections(std::initializer_list<Intersection> list);
extern Intersection GetClosestIntersection(const std::vector<Intersection> &intersections);
extern Computations PrepareComputations(const Intersection &intersection, const Ray &ray);
extern std::vector<float> Intersect(const Sphere &sphere, const Ray &ray);
