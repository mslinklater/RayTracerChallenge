#include <catch2/catch_test_macros.hpp>
#include "canvas.hpp"
#include "utils.hpp"
#include <fstream>
#include <string>
#include <vector>

TEST_CASE("Create canvas", "[basic]")
{
    Canvas c{10, 20};
    REQUIRE(c.GetWidth() == 10);
    REQUIRE(c.GetHeight() == 20);

    for (int y = 0; y < c.GetHeight(); ++y)
    {
        for (int x = 0; x < c.GetWidth(); ++x)
        {
            REQUIRE(c.GetPixel(x, y) == Color{0.f, 0.f, 0.f});
        }
    }
}

TEST_CASE("Write pixel to canvas", "[basic]")
{
    Canvas c{10, 20};
    Color red{1.f, 0.f, 0.f};
    c.WritePixel(2, 3, red);
    REQUIRE(c.GetPixel(2, 3) == red);
}

TEST_CASE("Pixels outside canvas bounds", "[basic]")
{
    Canvas c{10, 20};
    Color red{1.f, 0.f, 0.f};
    c.WritePixel(-1, -1, red);
    c.WritePixel(10, 20, red);
    REQUIRE(c.GetPixel(-1, -1) == Color{0.f, 0.f, 0.f});
    REQUIRE(c.GetPixel(10, 20) == Color{0.f, 0.f, 0.f});
}

TEST_CASE("Pixels on canvas bounds", "[basic]")
{
    Canvas c{10, 20};
    Color red{1.f, 0.f, 0.f};
    c.WritePixel(0, 0, red);
    c.WritePixel(9, 19, red);
    REQUIRE(c.GetPixel(0, 0) == red);
    REQUIRE(c.GetPixel(9, 19) == red);
}

// PPM file output tests

TEST_CASE("Write canvas to PPM file", "[basic]")
{
    Canvas c{5, 3};
    c.WriteToPPM("test.ppm");

    auto lines = ReadLinesFromFile("test.ppm");

    REQUIRE(lines.size() >= 3);
    REQUIRE(lines[0] == "P3");
    REQUIRE(lines[1] == "5 3");
    REQUIRE(lines[2] == "255");
}

TEST_CASE("PPM file pixel data", "[basic]")
{
    Canvas c{5, 3};
    Color c1{1.5f, 0.f, 0.f};
    Color c2{0.f, 0.5f, 0.f};
    Color c3{-0.5f, 0.f, 1.f};

    c.WritePixel(0, 0, c1);
    c.WritePixel(2, 1, c2);
    c.WritePixel(4, 2, c3);

    c.WriteToPPM("test.ppm");

    auto lines = ReadLinesFromFile("test.ppm");

    REQUIRE(lines.size() >= 6);
    REQUIRE(lines[3] == "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0");
    REQUIRE(lines[4] == "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0");
    REQUIRE(lines[5] == "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255");
}

TEST_CASE("PPM file pixel data with line breaks", "[basic]")
{
    Canvas c{10, 2};
    for (int x = 0; x < c.GetWidth(); ++x)
    {
        for (int y = 0; y < c.GetHeight(); ++y)
        {
            c.WritePixel(x, y, Color{1.f, 0.8f, 0.6f});
        }
    }

    c.WriteToPPM("test.ppm");

    auto lines = ReadLinesFromFile("test.ppm");

    REQUIRE(lines.size() >= 4);
    REQUIRE(lines[3] == "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204");
    REQUIRE(lines[4] == "153 255 204 153 255 204 153 255 204 153 255 204 153");
}