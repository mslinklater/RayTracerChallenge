#pragma once
#include <filesystem>

namespace Filesystem
{
/**
 * @brief Finds the filepath of a file, searching up the tree and in common locations. Returns an empty path if the file
 * cannot be found.
 * @params filename The name of the file to find.
 * @returns The filepath of the file, or an empty path if the file cannot be found
 */
std::filesystem::path FindFile(const std::string& filename);
} // namespace Filesystem
