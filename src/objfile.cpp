#include "objfile.hpp"
#include <filesystem>
#include <fstream>

ObjFile::ObjFile(const std::string& filename)
{
    if (!std::filesystem::exists(filename))
    {
        throw std::runtime_error("File not found: " + filename);
    }
}
