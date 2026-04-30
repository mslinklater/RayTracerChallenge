#include "objfile.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Correctly reports being unable to find the file", "[obj_file]")
{
    // throws an exception if the file cannot be found

    REQUIRE_THROWS(ObjFile("nonexistent.obj"));
}

TEST_CASE("Ignoring unrecognised lines", "[obj_file]")
{
    ObjFile obj("testdata/objfiles/unrecognised_lines.obj");
}
