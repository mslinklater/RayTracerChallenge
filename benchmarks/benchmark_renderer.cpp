#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>

#include "camera.hpp"
#include "computations.hpp"
#include "maths.hpp"
#include "renderer.hpp"
#include "world.hpp"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace
{
constexpr int kBenchmarkWidth = 48;
constexpr int kBenchmarkHeight = 24;

Camera MakeBenchmarkCamera()
{
    Camera camera(kBenchmarkWidth, kBenchmarkHeight, kPi / 3.f);
    camera.SetTransform(Matrix::ViewTransform(Point(0.f, 1.5f, -5.f), Point(0.f, 1.f, 0.f), Vector(0.f, 1.f, 0.f)));
    return camera;
}

void WriteReport(const std::filesystem::path &outputPath, const char *reportTemplate,
                 const std::vector<ankerl::nanobench::Result> &results)
{
    std::ofstream output(outputPath);
    if (!output)
    {
        throw std::runtime_error("Failed to open benchmark report output: " + outputPath.string());
    }

    ankerl::nanobench::render(reportTemplate, results, output);
}

void WriteReports(const std::filesystem::path &outputDirectory, const std::vector<ankerl::nanobench::Result> &results)
{
    std::filesystem::create_directories(outputDirectory);
    WriteReport(outputDirectory / "raytracer-benchmarks.csv", ankerl::nanobench::templates::csv(), results);
    WriteReport(outputDirectory / "raytracer-benchmarks.json", ankerl::nanobench::templates::json(), results);
    WriteReport(outputDirectory / "raytracer-benchmarks.html", ankerl::nanobench::templates::htmlBoxplot(), results);
}
} // namespace

int main(int argc, char **argv)
{
    const std::filesystem::path outputDirectory = argc > 1 ? std::filesystem::path(argv[1]) : std::filesystem::path("benchmarks");

    const World world = Renderer::DefaultWorld();
    const Camera camera = MakeBenchmarkCamera();
    const Ray centerRay = camera.RayForPixel(kBenchmarkWidth / 2, kBenchmarkHeight / 2);
    const IntersectionVector intersections = Renderer::IntersectWorld(world, centerRay);
    const Intersection hit = Renderer::GetClosestIntersection(intersections);
    const Computations comps = Renderer::PrepareComputations(hit, centerRay, world, &intersections);

    ankerl::nanobench::Bench fastBench;
    fastBench.minEpochTime(std::chrono::milliseconds(50));
    fastBench.minEpochIterations(100000);

    fastBench.run("IntersectWorld center ray", [&] {
        const IntersectionVector result = Renderer::IntersectWorld(world, centerRay);
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    fastBench.run("ShadeHit center intersection", [&] {
        const Color result = Renderer::ShadeHit(world, comps, Renderer::kMaxRecursionDepth);
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    fastBench.run("ColorAt center ray", [&] {
        const Color result = Renderer::ColorAt(world, centerRay);
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    ankerl::nanobench::Bench renderBench;
    renderBench.minEpochTime(std::chrono::milliseconds(50));
    renderBench.minEpochIterations(500);

    renderBench.run("Render default world 48x24", [&] {
        const Canvas result = Renderer::Render(camera, world);
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    std::vector<ankerl::nanobench::Result> results = fastBench.results();
    results.insert(results.end(), renderBench.results().begin(), renderBench.results().end());

    WriteReports(outputDirectory, results);
    std::cout << "Benchmark reports written to " << outputDirectory << '\n';
    return 0;
}
