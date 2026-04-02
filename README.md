# RayTracerChallenge

## Build

```bash§
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE
cmake --build build
```

## Run tests

## TODO

* Find out why images generated via LazyVim seem to be stored somewhere else - maybe a CMake issue?
* Move stuff around into more sensible locations - groupings could be better
* Add output comparison tests - maybe wait until we have a scene description language
