# RayTracerChallenge

## Introduction

This is my implementation of the ray tracer described in the book "The Ray Tracer Challenge" by Jamis Buck.
It is implemented in C++ and uses CMake as a build system. Testing is done via the Catch2 testing framework.
I have added a load of documentation about how the ray tracer works. The maths behind the ray tracer aren't covered in much detail at all in the book, so I have added more in-depth explanations here. I have mainly done this for my own benefit, but I hope anyone reading this will also find the notes helpful.

## Build

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE
cmake --build build
```

The configure step will also fetch **JsonCpp** and, unless `-DBUILD_BENCHMARKS=OFF` is set, **nanobench** for the benchmark executable.

## Run benchmarks

```bash
cmake --build build --target RayTracerChallengeBenchmarks
cmake --build build --target benchmark-report
```

This writes terminal output plus CSV, JSON, and HTML benchmark reports to `build/benchmarks/`.

## Run tests

For a full test coverage report in HTML format run the following

```
cmake -S . -B build-coverage -G Ninja -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON && cmake --build build-coverage && cmake --build build-coverage --target coverage-html
```

## JSON syntax examples

The project now pulls in **JsonCpp** automatically through CMake. Include it like this:

```cpp
#include <json/json.h>
```

### Serialize to JSON

```cpp
Json::Value root;
root["camera"]["width"] = 160;
root["camera"]["height"] = 90;
root["camera"]["fieldOfView"] = 1.0472f;
root["objects"][0]["type"] = "sphere";
root["objects"][0]["material"]["reflective"] = 0.5f;

Json::StreamWriterBuilder writerBuilder;
writerBuilder["indentation"] = "  ";
std::string json = Json::writeString(writerBuilder, root);
```

### Deserialize from JSON

```cpp
std::string json = R"json(
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

bool ok = Json::parseFromStream(readerBuilder, input, &root, &errors);
```

Useful accessors:

```cpp
int width = root["camera"]["width"].asInt();
float fieldOfView = root["camera"]["fieldOfView"].asFloat();
std::string pattern = root["material"]["pattern"].asString();
```

## TODO

* Test multiple lights - AI added this so needs testing
* Add output comparison tests - maybe wait until we have a scene description language
* Add chaining setters to the structure based classes like Material

## Misc

* Files is the parked/ folder are not included in the build but are interesting things I should look at

## Further Work

* Add more patterns - p139-140
* Currently transparent objects create solid shadows - this needs fixing
* Add a spacial partition system to accelerate ray/object intersections - p145-146
* Once the book is complete do a refactor and rename of everything - not a fan of current naming
* Add the ray tracer code from 'Ray Tracing in One Weekend' - find areas of overlap and contrast
* Add scene description language - JSON
* Add ImGUI support for real time rendering and scene editing
* Add a physics engine - the one from 'Physics Engine in a Weekend' would be a good starting point
* Add a Vulkan renderer

## Refactoring

```
code
    core
        src
        include
        tests
    editor
        src
        include
        tests
    app
        src
        include
        tests
```
