# Copilot Instructions

This is a C++20 implementation of [The Ray Tracer Challenge](http://raytracerchallenge.com/) book, built incrementally chapter by chapter.

## Build & Test

```bash
# Configure
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE

# Build
cmake --build build

# Run all tests
cmake --build build --target test
# or
cd build && ctest --output-on-failure

# Run a single test file (from build dir)
cd build && ./tests/RayTracerTests "[tuples]"   # Catch2 tag filter
cd build && ./tests/RayTracerTests "test name"  # exact name match

# Coverage build
cmake -S . -B build-coverage -DENABLE_COVERAGE=ON
cmake --build build-coverage --target coverage-html  # HTML report → build-coverage/html/
```

## Architecture

All types are header-declared in `include/` with implementations in `src/`. The chapter executables (`chapter4.cpp` through `chapter7.cpp`) are standalone demos that write PPM images to `../images/`.

**Type hierarchy:**
- `Tuple` — base 4D type (x, y, z, w); `w=1` → point, `w=0` → vector
- `Color` — RGB triple
- `Matrix` — NxN transformation matrix (typically 4×4); used for all spatial transforms
- `Ray` — origin + direction `Tuple`; transformable by `Matrix`
- `Sphere` — has a `Matrix` transform and a `Material`; supports `Intersect(ray)` and `NormalAt(point)`
- `Intersection` — a `(t, Sphere*)` pair; `Computations` holds pre-shading data derived from one
- `Canvas` — pixel buffer with `WriteToPPM(filename)`
- `Camera` — FOV + transform → generates rays via `RayForPixel(px, py)`
- `World` — owns a `deque<Sphere>` and `vector<Light>`
- `renderer.hpp` — free functions: `Render`, `ColorAt`, `IntersectWorld`, `ShadeHit`, `Lighting`, `PrepareComputations`

## Key Conventions

**Naming:**
- Types and methods: `PascalCase` (`Tuple`, `NormalAt`, `WritePixel`)
- Local variables: `camelCase`
- Constants: `kCamelCase` (`kDegreesToRadians`)

**Operator overloading on math types:**
- `|` → dot product (`a | b`)
- `^` → cross product (`a ^ b`)
- `*` between `Ray` and `Matrix` → transformed ray

**Mutable access pattern** — types expose separate const and mutable getters:
```cpp
sphere.GetMaterial();          // const ref
sphere.GetMutableMaterial();   // non-const ref for modification
```

**Floating-point equality** — always use `AreEqual(a, b)` (tolerance `0.0001f`) from `maths.hpp`, never `==` on floats directly. Equality operators on `Tuple`, `Color`, and `Matrix` already use this internally.

**Testing** — Catch2 3; one `TEST_CASE` per behaviour, grouped by tag matching the type name (e.g., `[tuples]`, `[matrix]`). Tests live in `tests/test_<type>.cpp`.
