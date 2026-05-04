#pragma once

#include "objfile.hpp"
#include "shapes/group.hpp"
#include "shapes/sphere.hpp"
#include <string>
#include <vector>

/**
 * @brief Clamps a normalised colour channel to the integer range [0, 255].
 * @param value A colour channel value in [0, 1] (clamped if out of range).
 * @return An integer in [0, 255] suitable for PPM output.
 */
extern int ToPPMChannel(float value);

/**
 * @brief Reads all lines of a text file into a vector of strings.
 * @param filename Path to the file to read.
 * @return A vector where each element is one line from the file (without newline).
 */
extern std::vector<std::string> ReadLinesFromFile(const std::string& filename);

/**
 * @brief Creates a unit sphere with a glass-like material preset.
 *
 * Transparency and refractive index are set to model glass (n ≈ 1.5).
 * @param name Human-readable identifier for the sphere.
 * @return A configured @c Sphere with glass material properties.
 */
extern Sphere GlassSphere(const std::string& name);

/**
 * @brief Converts an OBJ file into scene-ready groups of triangles.
 *
 * Each parsed OBJ group becomes a @c Group shape. Each triangulated face in that
 * OBJ group becomes a child @c Triangle shape within the corresponding group.
 * Empty OBJ groups are omitted.
 *
 * @param filename Path to the OBJ file.
 * @return A vector of groups ready to be added to a @c World.
 */
extern std::vector<Group> ObjFileToGroups(const std::string& filename);
