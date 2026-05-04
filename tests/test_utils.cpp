#include <catch2/catch_test_macros.hpp>
#include "maths.hpp"
#include "shapes/triangle.hpp"
#include "utils.hpp"
#include <filesystem>
#include <fstream>

// Tests for AreEqual

TEST_CASE("AreEqual with equal values", "[utils]")
{
    REQUIRE(AreEqual(0.1f + 0.2f, 0.3f) == true);
}

TEST_CASE("AreEqual with different values", "[utils]")
{
    REQUIRE(AreEqual(0.1f + 0.2f, 0.31000f) == false);
}

TEST_CASE("AreEqual with very close values", "[utils]")
{
    REQUIRE(AreEqual(0.1f + 0.2f, 0.300001f) == true);
}

// Tests for ToPPMChannel

TEST_CASE("ToPPMChannel with value in range [0, 1]", "[utils]")
{
    REQUIRE(ToPPMChannel(0.5f) == 128);
}

TEST_CASE("ToPPMChannel with valid floor value", "[utils]")
{
    REQUIRE(ToPPMChannel(0.0f) == 0);
}

TEST_CASE("ToPPMChannel with valid ceiling value", "[utils]")
{
    REQUIRE(ToPPMChannel(1.0f) == 255);
}

TEST_CASE("ToPPMChannel with value below the floor", "[utils]")
{
    REQUIRE(ToPPMChannel(-0.1f) == 0);
}

TEST_CASE("ToPPMChannel with value above the ceiling", "[utils]")
{
    REQUIRE(ToPPMChannel(1.1f) == 255);
}

// Tests for ReadLinesFromFile

TEST_CASE("ReadFileAsLines with non-existent file", "[utils]")
{
    auto lines = ReadLinesFromFile("non_existent_file.txt");
    REQUIRE(lines.empty());
}

TEST_CASE("ReadFileAsLines with a valid file", "[utils]")
{
    // Create a temporary file with known content
    const std::string filename = (std::filesystem::temp_directory_path() / "raytracerchallenge_valid_lines.txt").string();
    std::ofstream outFile(filename);
    outFile << "Line 1\nLine 2\nLine 3";
    outFile.close();

    auto lines = ReadLinesFromFile(filename);
    REQUIRE(lines.size() == 3);
    REQUIRE(lines[0] == "Line 1");
    REQUIRE(lines[1] == "Line 2");
    REQUIRE(lines[2] == "Line 3");

    // Clean up the temporary file
    std::remove(filename.c_str());
}

TEST_CASE("ReadFileAsLines with an empty file", "[utils]")
{
    // Create an empty temporary file
    const std::string filename = (std::filesystem::temp_directory_path() / "raytracerchallenge_empty_lines.txt").string();
    std::ofstream outFile(filename);
    outFile.close();

    auto lines = ReadLinesFromFile(filename);
    REQUIRE(lines.empty());

    // Clean up the temporary file
    std::remove(filename.c_str());
}

TEST_CASE("ObjFileToGroups creates a group hierarchy from an obj file", "[utils]")
{
    const std::vector<Group> groups = ObjFileToGroups("testdata/objfiles/triangles.obj");

    REQUIRE(groups.size() == 2);
    REQUIRE(groups[0].GetName() == "FirstGroup");
    REQUIRE(groups[1].GetName() == "SecondGroup");
    REQUIRE(groups[0].GetNumChildren() == 1);
    REQUIRE(groups[1].GetNumChildren() == 1);

    const auto* firstTriangle = dynamic_cast<const Triangle*>(&groups[0].GetChild(0));
    const auto* secondTriangle = dynamic_cast<const Triangle*>(&groups[1].GetChild(0));

    REQUIRE(firstTriangle != nullptr);
    REQUIRE(secondTriangle != nullptr);
    REQUIRE(firstTriangle->GetP1() == Point(-1.f, 1.f, 0.f));
    REQUIRE(firstTriangle->GetP2() == Point(-1.f, 0.f, 0.f));
    REQUIRE(firstTriangle->GetP3() == Point(1.f, 0.f, 0.f));
    REQUIRE(secondTriangle->GetP1() == Point(-1.f, 1.f, 0.f));
    REQUIRE(secondTriangle->GetP2() == Point(1.f, 0.f, 0.f));
    REQUIRE(secondTriangle->GetP3() == Point(1.f, 1.f, 0.f));
}
