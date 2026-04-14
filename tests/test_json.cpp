#include <catch2/catch_test_macros.hpp>
#include "maths.hpp"
#include <json/json.h>
#include <sstream>
#include <string>

TEST_CASE("JsonCpp serializes scene-style data", "[json]")
{
    Json::Value root;
    root["camera"]["width"] = 160;
    root["camera"]["height"] = 90;
    root["camera"]["fieldOfView"] = 1.0472f;
    root["objects"][0]["type"] = "sphere";
    root["objects"][0]["material"]["reflective"] = 0.5f;

    Json::StreamWriterBuilder writerBuilder;
    writerBuilder["indentation"] = "  ";

    const std::string json = Json::writeString(writerBuilder, root);

    REQUIRE(json.find("\"camera\"") != std::string::npos);
    REQUIRE(json.find("\"width\" : 160") != std::string::npos);
    REQUIRE(json.find("\"reflective\" : 0.5") != std::string::npos);
}

TEST_CASE("JsonCpp deserializes scene-style data", "[json]")
{
    const std::string json = R"json(
{
  "camera": {
    "width": 200,
    "height": 100,
    "fieldOfView": 0.785398
  },
  "material": {
    "pattern": "stripe",
    "reflective": 0.25
  }
}
)json";

    Json::CharReaderBuilder readerBuilder;
    Json::Value root;
    std::string errors;
    std::istringstream input(json);

    REQUIRE(Json::parseFromStream(readerBuilder, input, &root, &errors));
    REQUIRE(errors.empty());
    REQUIRE(root["camera"]["width"].asInt() == 200);
    REQUIRE(root["camera"]["height"].asInt() == 100);
    REQUIRE(AreEqual(root["camera"]["fieldOfView"].asFloat(), 0.785398f));
    REQUIRE(root["material"]["pattern"].asString() == "stripe");
    REQUIRE(AreEqual(root["material"]["reflective"].asFloat(), 0.25f));
}
