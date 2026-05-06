#pragma once
#include "types.hpp"
#include <cassert>
#include <cstddef>

/**
 * @brief Records a single ray-object intersection as a @c (t, objectId) pair.
 *
 * @c t is the ray parameter at the hit point (i.e., @c ray.PositionAt(t)).
 * An @c objectId of @c kInvalidObjectId indicates a miss.
 */
class Intersection
{
  public:
    /**
     * @brief Constructs an intersection record.
     */
    Intersection();

    /**
     * @brief Constructs an intersection record.
     * @param t        Ray parameter at the hit point.
     * @param objectId Stable world ID of the intersected shape.
     */
    Intersection(float t, ObjectId objectId);

    /**
     * @brief Constructs an intersection record.
     * @param t        Ray parameter at the hit point.
     * @param objectId Stable world ID of the intersected shape.
     * @param u        Barycentric coordinate u at the intersection point.
     * @param v        Barycentric coordinate v at the intersection point.
     */
    Intersection(float t, ObjectId objectId, float u, float v);

    /** @brief Returns the ray parameter at the intersection point. */
    float GetT() const;

    float GetU() const;
    float GetV() const;

    /** @brief Returns the world object ID of the intersected shape. */
    ObjectId GetObjectId() const;

    /** @brief Equality comparison. */
    bool operator==(const Intersection& other) const;

  protected:
    float t = 0.f;                        ///< The distance along the ray to the intersection point.
    ObjectId objectId = kInvalidObjectId; ///< Stable ID of the intersected object; kInvalidObjectId means no hit.
    float u = 0.f;                        ///< Barycentric coordinate u at the intersection point.
    float v = 0.f;                        ///< Barycentric coordinate v at the intersection point.
};
