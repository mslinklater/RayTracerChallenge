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

For a full test coverage report in HTML format run the following

```
cmake -S . -B build-coverage -G Ninja -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON && cmake --build build-coverage && cmake --build build-coverage --target coverage-html
```

## TODO

* Move stuff around into more sensible locations - groupings could be better
* Add output comparison tests - maybe wait until we have a scene description language
* Add the concept of units to all measurements - mm, cm, m etc. Make new types and make arithmetic between them automatic, using custom operators - can we even do this reliably with C++ ?

## Misc

* Files is the parked/ folder are not included in the build but are interesting things I should look at

## Further Work

* Add more patterns - p139-140
