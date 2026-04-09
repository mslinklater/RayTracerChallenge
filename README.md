# RayTracerChallenge

## Introduction

This is my implementation of the ray tracer described in the book "The Ray Tracer Challenge" by Jamis Buck.
It is implemented in C++ and uses CMake as a build system. Testing is done via the Catch2 testing framework.

## Build

```bash§
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE
cmake --build build
```

## Run tests

## TODO

* Move stuff around into more sensible locations - groupings could be better
* Add output comparison tests - maybe wait until we have a scene description language

## Further Work

* Add more patterns - p139-140
