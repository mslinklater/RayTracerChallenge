#include "objfile.hpp"
#include "shapes/group.hpp"
#include "utils.hpp"
#include "world.hpp"
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

    ObjFile::GroupIndex groupIndex = obj.GetDefaultGroupIndex();

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
    ObjFile::GroupIndex groupIndex = obj.GetDefaultGroupIndex();

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

TEST_CASE("Triangles in groups", "[obj_file]")
{
    ObjFile obj("testdata/objfiles/triangles.obj");
    ObjFile::GroupIndex g1 = obj.GetGroupIndex("FirstGroup");
    ObjFile::GroupIndex g2 = obj.GetGroupIndex("SecondGroup");
    const ObjFileTriangle& t1 = obj.GetTriangle(g1, 1);
    const ObjFileTriangle& t2 = obj.GetTriangle(g2, 1);
    REQUIRE(*t1.p1 == obj.GetVertex(1));
    REQUIRE(*t1.p2 == obj.GetVertex(2));
    REQUIRE(*t1.p3 == obj.GetVertex(3));
    REQUIRE(*t2.p1 == obj.GetVertex(1));
    REQUIRE(*t2.p2 == obj.GetVertex(3));
    REQUIRE(*t2.p3 == obj.GetVertex(4));
}

TEST_CASE("Converting an OBJ file to a group", "[obj_file]")
{
    std::vector<Group> groups = ObjFileToGroups("testdata/objfiles/triangles.obj");
    bool bFoundFirstGroup = false;
    bool bFoundSecondGroup = false;

    for (auto group : groups)
    {
        if (group.GetName() == "FirstGroup")
        {
            bFoundFirstGroup = true;
        }
        else if (group.GetName() == "SecondGroup")
        {
            bFoundSecondGroup = true;
        }
    }
    REQUIRE(bFoundFirstGroup);
    REQUIRE(bFoundSecondGroup);
}

TEST_CASE("Vertex normal records", "[obj_file]")
{
    ObjFile obj("testdata/objfiles/contains_normals.obj");

    REQUIRE(obj.GetNumVertexNormals() == 3);
    REQUIRE(obj.GetVertexNormal(1) == ObjFileVertexNormal(0.0f, 0.0f, 1.0f));
    REQUIRE(obj.GetVertexNormal(2) == ObjFileVertexNormal(0.707f, 0.0f, -0.707f));
    REQUIRE(obj.GetVertexNormal(3) == ObjFileVertexNormal(1.0f, 2.0f, 3.0f));
}
