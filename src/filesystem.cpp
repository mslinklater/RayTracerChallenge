#include "filesystem.hpp"

std::filesystem::path Filesystem::FindFile(const std::string& filename)
{
    // Check the current directory
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path filePath = currentPath / filename;
    if (std::filesystem::exists(filePath))
    {
        return filePath;
    }

    // Check parent directories
    for (auto it = currentPath; it.has_parent_path() && it != it.parent_path(); it = it.parent_path())
    {
        filePath = it / filename;
        if (std::filesystem::exists(filePath))
        {
            return filePath;
        }
    }

    // Check common locations (e.g., "data" directory)
    filePath = currentPath / "data" / filename;
    if (std::filesystem::exists(filePath))
    {
        return filePath;
    }

    // File not found
    return std::filesystem::path();
}
