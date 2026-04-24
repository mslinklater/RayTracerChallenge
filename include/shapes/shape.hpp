#pragma once

#include "intersection.hpp"
#include "material.hpp"
#include "matrix.hpp"
#include "ray.hpp"
#include "types.hpp"

#include <atomic>
#include <cassert>
#include <mutex>
#include <string>

/**
 * @brief Abstract base class for all renderable geometric shapes.
 *
 * Shapes hold a world-space transform, a @c Material, and a unique @c ObjectId
 * assigned by the @c World. Subclasses implement @c IntersectLocal and
 * @c NormalAtLocal to define their geometry in object (local) space;
 * @c Shape::Intersect and @c Shape::NormalAt handle the coordinate transforms.
 */
class Shape
{
  public:
    /**
     * @brief Constructs a shape with an identity transform and default material.
     * @param name Human-readable identifier for debugging and lookup.
     */
    Shape(const std::string& name);
    Shape(const Shape& other);
    Shape& operator=(const Shape& other);
    virtual ~Shape() = default;

    /**
     * @brief Sets the unique world object ID (assigned by @c World::AddObject).
     * @param id The ID to assign.
     */
    void SetWorldObjectId(ObjectId id);

    /** @brief Returns the unique world object ID. */
    ObjectId GetWorldObjectId() const;

    /** @brief Returns a mutable reference to the material for in-place modification. */
    Material& GetMutableMaterial();

    /** @brief Returns a const reference to the material. */
    const Material& GetMaterial() const;

    /** @brief Replaces the material. */
    void SetMaterial(const Material& m);

    /** @brief Returns a const reference to the object-to-world transform. */
    const Matrix& GetTransform() const;

    /** @brief Returns a mutable reference to the object-to-world transform. */
    Matrix& GetMutableTransform();

    /** @brief Replaces the object-to-world transform. */
    void SetTransform(const Matrix& t);

    /** @brief Returns the human-readable name of the shape. */
    const std::string& GetName() const;

    /**
     * @brief Returns the world-space surface normal at @p point.
     *
     * Converts @p point to object space, delegates to @c NormalAtLocal, then
     * transforms the result back to world space.
     * @param point A world-space point on the surface of the shape.
     */
    Tuple NormalAt(const Tuple& point) const;

    /** @brief Converts a world-space point into this shape's object space. */
    Tuple WorldToObject(const Tuple& point) const;

    /**
     * @brief Returns the object-space surface normal at a local-space @p point.
     *
     * Override in subclasses to define the geometry-specific normal.
     * @param point A point already in the shape's local (object) space.
     */
    virtual Tuple NormalAtLocal(const Tuple& point) const;

    /**
     * @brief Intersects @p ray (in world space) with this shape.
     *
     * Transforms the ray into object space and delegates to @c IntersectLocal.
     * @param ray A ray in world space.
     * @return A list of ray parameters @c t at each intersection point.
     */
    // std::vector<float> Intersect(const Ray& ray) const;
    std::vector<Intersection> Intersect(const Ray& ray) const;

    /**
     * @brief Intersects @p ray (already in object/local space) with this shape.
     *
     * Override in subclasses to define the geometry-specific intersection test.
     * @param ray A ray already transformed into the shape's local space.
     * @return A list of ray parameters @c t at each intersection point.
     */
    virtual std::vector<Intersection> IntersectLocal(const Ray& ray) const;

    /**
     * @brief Returns a pointer to the parent shape, or nullptr if this is a top-level shape.
     * @return Pointer to the parent shape, or nullptr if no parent.
     */
    Shape* GetParent() const;

    /**
     * @brief Sets the parent shape for this shape.
     * @param parent Pointer to the parent shape (must not be nullptr).
     */
    void SetParent(Shape* parent);

  protected:
    void UpdateTransformCache() const;
    void EnsureTransformCache() const;

    ObjectId worldObjectId = kInvalidObjectId; ///< Unique world ID assigned by @c World.

    std::string name;    ///< Human-readable identifier.
    Matrix transform{4}; ///< Object-to-world transformation matrix (default: identity).
    Material material;   ///< Surface material properties.

    Shape* parent = nullptr; ///< Parent shape (nullptr if top-level).

    // mutable cached values
    mutable Matrix inverseTransform{4};          ///< Cached inverse transform.
    mutable Matrix inverseTransposeTransform{4}; ///< Cached inverse-transpose transform.
    mutable std::mutex transformCacheMutex;      ///< Guards lazy cache refresh after direct transform mutation.
    mutable std::atomic<bool> transformCacheValidAtomic{false}; ///< Whether the cached transform matrices are current.
};

/// @brief Owning pointer to a heap-allocated @c Shape.
using ShapeUniquePtr = std::unique_ptr<Shape>;
using ShapeSharedPtr = std::shared_ptr<Shape>;
