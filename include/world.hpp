#pragma once
#include "light.hpp"
#include "shapes/shape.hpp"
#include "types.hpp"
#include <cassert>
#include <deque>
#include <memory>
#include <type_traits>
#include <vector>

/**
 * @brief Scene container that owns all shapes and lights.
 *
 * Shapes are stored as owning @c ShapePtr (unique_ptr<Shape>) in a deque to
 * keep pointer stability as objects are added. Each shape receives a unique
 * @c ObjectId when added. Lights are stored by value in a vector.
 */
class World
{
  public:
    /** @brief Constructs an empty world with no objects or lights. */
    World() = default;

    /** @brief Returns a const reference to the deque of all shapes. */
    const std::deque<ShapePtr> &GetObjects() const;

    /** @brief Returns a const reference to the list of all lights. */
    const std::vector<Light> &GetLights() const;

    /**
     * @brief Replaces the light at @p index with @p light.
     * @param index Zero-based index into the lights list.
     * @param light The replacement light.
     */
    void ReplaceLight(int index, const Light &light);

    /**
     * @brief Adds a point light to the world.
     * @param light The light to add.
     */
    void AddLight(const Light &light)
    {
        assert(light.position.IsValid());
        assert(light.position.IsPoint());
        assert(std::isfinite(light.intensity.r));
        assert(std::isfinite(light.intensity.g));
        assert(std::isfinite(light.intensity.b));
        lights.push_back(light);
    }

    /**
     * @brief Adds any @c Shape subclass to the world and assigns it a unique ID.
     *
     * The object is copied into a heap-allocated @c ShapePtr. The caller's copy
     * of @p object has its world ID updated so it can be used for subsequent lookups.
     *
     * @tparam T A concrete subclass of @c Shape.
     * @param object The shape to add (will be copied).
     * @return The @c ObjectId assigned to the newly added shape.
     */
    template <typename T, typename = std::enable_if_t<std::is_base_of_v<Shape, T>>> ObjectId AddObject(T &object)
    {
        assert(!object.GetName().empty());
        object.SetWorldObjectId(nextObjectId++);
        ShapePtr shapePtr = std::make_unique<T>(object);
        AddObjectImpl(std::move(shapePtr));
        return object.GetWorldObjectId();
    }

    /**
     * @brief Returns a const reference to the shape with the given @p id.
     * @param id A valid @c ObjectId previously returned by @c AddObject.
     */
    const Shape &GetObject(ObjectId id) const;

    /**
     * @brief Returns a const reference to the first shape with the given @p name.
     * @param name The human-readable name to look up.
     */
    const Shape &GetObjectWithName(const std::string &name) const;

    /**
     * @brief Returns a const reference to the light at @p index.
     * @param index Zero-based index into the lights list.
     */
    const Light &GetLight(size_t index) const;

    /**
     * @brief Returns a mutable reference to the shape with the given @p id.
     * @param id A valid @c ObjectId previously returned by @c AddObject.
     */
    Shape &GetMutableObject(ObjectId id);

    /**
     * @brief Returns a mutable reference to the first shape with the given @p name.
     * @param name The human-readable name to look up.
     */
    Shape &GetMutableObjectWithName(const std::string &name) const;

    /**
     * @brief Returns a mutable reference to the light at @p index.
     * @param index Zero-based index into the lights list.
     */
    Light &GetMutableLight(size_t index);

    /**
     * @brief Returns true if @p light (by value) is present in the world.
     * @param light The light to search for.
     */
    bool ContainsLight(const Light &light) const;

    /**
     * @brief Returns true if a shape equivalent to @p object is present.
     * @param object The shape to search for (compared by ObjectId).
     */
    bool ContainsObject(const Shape &object) const;

  private:
    /** @brief Internal helper that stores a pre-built @c ShapePtr in the deque. */
    void AddObjectImpl(ShapePtr ptr);

    std::deque<ShapePtr> objects; ///< Owning storage for all shapes in the scene.
    std::vector<Light> lights;    ///< All lights in the scene.

    ObjectId nextObjectId = 9; ///< Monotonically increasing ID counter.
};
