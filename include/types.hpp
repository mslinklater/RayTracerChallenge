#pragma once
#include <cstddef>

/// @brief Unique identifier for scene objects within a @c World.
using ObjectId = std::size_t;

/// @brief Sentinel value representing an unassigned or invalid @c ObjectId.
static constexpr ObjectId kInvalidObjectId = static_cast<ObjectId>(-1);
