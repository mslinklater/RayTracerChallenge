#pragma once
#include "canvas.hpp"
#include "intersection.hpp"

class Camera;
class World;
class Ray;

extern Canvas Render(const Camera &camera, const World &world);
extern Color ColorAt(const World &world, const Ray &ray);
extern std::vector<Intersection> IntersectWorld(const World &world, const Ray &ray);
extern Color ShadeHit(const World &world, const Computations &comps);
