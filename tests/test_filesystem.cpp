#include "filesystem.hpp"
#include <catch2/catch_test_macros.hpp>
#include <filesystem>

TEST_CASE("Filesystem::FindFile finds existing file", "[filesystem]")
{
    std::filesystem::path filePath = Filesystem::FindFile("testdata/filesystem/find_file.txt");
    REQUIRE(!filePath.empty());
    REQUIRE(std::filesystem::exists(filePath));
}

TEST_CASE("Filesystem::FindFile returns empty path for non-existent file", "[filesystem]")
{
    std::filesystem::path filePath = Filesystem::FindFile("non_existent_file.txt");
    REQUIRE(filePath.empty());
}
