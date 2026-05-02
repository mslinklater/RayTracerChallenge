#include "objfile.hpp"
#include "tuple.hpp"
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

TEST_CASE("Vertex records", "[obj_file]")
{
    ObjFile obj("testdata/objfiles/vertex_records.obj");
    REQUIRE(obj.GetNumVertices() == 4);
    REQUIRE(obj.GetVertex(1) == ObjFileVertex(-1.0f, 1.0f, 0.0f));
    REQUIRE(obj.GetVertex(2) == ObjFileVertex(-1.0f, 0.5f, 0.0f));
    REQUIRE(obj.GetVertex(3) == ObjFileVertex(1.0f, 0.0f, 0.0f));
    REQUIRE(obj.GetVertex(4) == ObjFileVertex(1.0f, 1.0f, 0.0f));
}

TEST_CASE("Parsing triangle faces", "[obj_file]")
{
    ObjFile obj("testdata/objfiles/triangle_faces.obj");

    uint32_t groupIndex = obj.GetDefaultGroup();

    const ObjFileTriangle& t1 = obj.GetTriangle(groupIndex, 1);
    const ObjFileTriangle& t2 = obj.GetTriangle(groupIndex, 2);
    REQUIRE(*t1.p1 == obj.GetVertex(1));
    REQUIRE(*t1.p2 == obj.GetVertex(2));
    REQUIRE(*t1.p3 == obj.GetVertex(3));
    REQUIRE(*t2.p1 == obj.GetVertex(1));
    REQUIRE(*t2.p2 == obj.GetVertex(3));
    REQUIRE(*t2.p3 == obj.GetVertex(4));
}

TEST_CASE("Triangulating polygons", "[obj_file]")
{
    ObjFile obj("testdata/objfiles/polygon_faces.obj");
    uint32_t groupIndex = obj.GetDefaultGroup();
    const ObjFileTriangle& t1 = obj.GetTriangle(groupIndex, 1);
    const ObjFileTriangle& t2 = obj.GetTriangle(groupIndex, 2);
    const ObjFileTriangle& t3 = obj.GetTriangle(groupIndex, 3);
    REQUIRE(*t1.p1 == obj.GetVertex(1));
    REQUIRE(*t1.p2 == obj.GetVertex(2));
    REQUIRE(*t1.p3 == obj.GetVertex(3));
    REQUIRE(*t2.p1 == obj.GetVertex(1));
    REQUIRE(*t2.p2 == obj.GetVertex(3));
    REQUIRE(*t2.p3 == obj.GetVertex(4));
    REQUIRE(*t3.p1 == obj.GetVertex(1));
    REQUIRE(*t3.p2 == obj.GetVertex(4));
    REQUIRE(*t3.p3 == obj.GetVertex(5));
}

#if 0
TEST_CASE("Triangles in groups", "[obj_file]")
{
    ObjFile obj("testdata/objfiles/triangles.obj");
    uint32_t g1 = obj.GetGroupIndex("FirstGroup");
    uint32_t g2 = obj.GetGroupIndex("SecondGroup");
    const ObjFileTriangle& t1 = obj.GetTriangle(g1, 1);
    const ObjFileTriangle& t2 = obj.GetTriangle(g2, 1);
    REQUIRE(*t1.p1 == obj.GetVertex(1));
    REQUIRE(*t1.p2 == obj.GetVertex(2));
    REQUIRE(*t1.p3 == obj.GetVertex(3));
    REQUIRE(*t2.p1 == obj.GetVertex(1));
    REQUIRE(*t2.p2 == obj.GetVertex(3));
    REQUIRE(*t2.p3 == obj.GetVertex(4));
}
#endif
