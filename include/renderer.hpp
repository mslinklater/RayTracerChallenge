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

/**
 * @brief Static utility class that implements the core ray tracing pipeline.
 *
 * All methods are static; the class cannot be instantiated. The main entry
 * point is @c Render, which drives the full pipeline from camera rays to a
 * finished @c Canvas. Individual pipeline steps are exposed as static methods
 * to support testing and composition.
 */
class Renderer
{
  public:
    /// @brief Default maximum recursion depth for reflected/refracted rays.
    constexpr static int kDefaultRemaining = 10;

    /**
     * @brief Renders the entire scene and returns the resulting image.
     * @param camera The camera defining resolution and view transform.
     * @param world  The scene to render.
     * @return A @c Canvas containing the rendered pixel colours.
     */
    static Canvas Render(const Camera &camera, const World &world);

    /**
     * @brief Computes the colour seen along @p ray within @p world.
     * @param world     The scene to query.
     * @param ray       The ray to trace.
     * @param remaining Maximum remaining recursion depth for reflections.
     * @return The colour at the first hit, or @c kBackgroundColor on a miss.
     */
    static Color ColorAt(const World &world, const Ray &ray, int remaining = 10);

    /**
     * @brief Collects all ray-shape intersections in @p world, sorted by @c t.
     * @param world The scene to intersect.
     * @param ray   The ray to test against all shapes.
     * @return A sorted list of all intersection records.
     */
    static IntersectionVector IntersectWorld(const World &world, const Ray &ray);

    /**
     * @brief Computes the colour contribution from reflected rays.
     * @param world     The scene (mutable to allow recursive shading).
     * @param comps     Pre-computed intersection data.
     * @param remaining Maximum remaining recursion depth.
     * @return The reflected colour, or black if the material is non-reflective or depth is exhausted.
     */
    static Color ReflectedColor(World &world, const Computations &comps, int remaining);

    /**
     * @brief Computes the colour contribution from refracted rays.
     * @param world     The scene (mutable to allow recursive shading).
     * @param comps     Pre-computed intersection data.
     * @param remaining Maximum remaining recursion depth.
     * @return The refracted colour, or black if the material is non-reflective or depth is exhausted.
     */
    static Color RefractedColor(World &world, const Computations &comps, int remaining);

    /**
     * @brief Computes the final shaded colour at a surface intersection.
     *
     * Accumulates contributions from all lights and adds the reflected colour.
     * @param world     The scene.
     * @param comps     Pre-computed intersection data.
     * @param remaining Maximum remaining recursion depth for reflections.
     * @return The shaded colour at the intersection.
     */
    static Color ShadeHit(const World &world, const Computations &comps, int remaining);

    /**
     * @brief Computes the Phong shading colour at a surface point.
     * @param material     The surface material.
     * @param object       The shape being shaded (used for pattern look-up).
     * @param light        The light source contributing to this calculation.
     * @param position     The world-space surface point.
     * @param eyeVector    The direction from the surface toward the eye.
     * @param normalVector The outward-facing surface normal.
     * @param inShadow     Whether the point is in shadow of @p light.
     * @return The Phong-shaded colour.
     */
    static Color Lighting(const Material &material, const Shape &object, const Light &light, const Tuple &position,
                          const Tuple &eyeVector, const Tuple &normalVector, EInShadow inShadow);

    /**
     * @brief Creates a default test world with two spheres and one point light.
     * @return A fully configured @c World ready for rendering.
     */
    static World DefaultWorld();

    /**
     * @brief Collects a brace-initialiser list of intersections into a sorted @c IntersectionVector.
     * @param list An initialiser list of @c Intersection values.
     * @return The intersections sorted by ascending @c t.
     */
    static IntersectionVector Intersections(std::initializer_list<Intersection> list);

    /**
     * @brief Returns the first positive-t intersection (the visible hit).
     * @param intersections A sorted list of intersections.
     * @return The closest intersection with t > 0, or an invalid intersection on miss.
     */
    static Intersection GetClosestIntersection(const IntersectionVector &intersections);

    /**
     * @brief Pre-computes shading data for @p intersection along @p ray.
     * @param intersection   The intersection to prepare.
     * @param ray            The ray that produced the intersection.
     * @param world          The scene (used to look up the intersected shape).
     * @param intersectionVec Optional full hit list used for computing n1/n2 refraction indices.
     * @return A populated @c Computations struct.
     */
    static Computations PrepareComputations(const Intersection &intersection, const Ray &ray, const World &world,
                                            const IntersectionVector *intersectionVec = nullptr);

    /**
     * @brief Determines whether @p point is in shadow with respect to the first light.
     * @param world The scene.
     * @param point The world-space point to test.
     * @return @c EInShadow::Yes if the point is in shadow, @c EInShadow::No otherwise.
     */
    static EInShadow IsShadowed(const World &world, const Tuple &point);

    /**
     * @brief Compute the Schlk approximation for reflectance at a surface intersection.
     * @param comps The pre-computed intersection data.
     */
    static float Schlick(const Computations &comps);

  private:
    Renderer() = default; ///< Non-instantiable static class.
};
