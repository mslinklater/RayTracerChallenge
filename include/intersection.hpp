#pragma once
#include "types.hpp"
#include <cstddef>
#include <vector>

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
     * @param t        Ray parameter at the hit point.
     * @param objectId Stable world ID of the intersected shape.
     */
    Intersection(float t, ObjectId objectId) : t(t), objectId(objectId)
    {
    }

    /** @brief Returns the ray parameter at the intersection point. */
    float GetT() const
    {
        return t;
    }

    /** @brief Returns the world object ID of the intersected shape. */
    ObjectId GetObjectId() const
    {
        return objectId;
    }

    /** @brief Equality comparison. */
    bool operator==(const Intersection &other) const;

  private:
    float t;           ///< The distance along the ray to the intersection point.
    ObjectId objectId; ///< Stable ID of the intersected object; kInvalidObjectId means no hit.
};

/// @brief Convenience alias for a list of intersections sorted by @c t.
typedef std::vector<Intersection> IntersectionVector;
