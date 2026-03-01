cmake_minimum_required(VERSION 3.16)
project(RayTracerChallenge CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Catch2 3 REQUIRED)

enable_testing()

# Main executable
add_executable(RayTracerChallenge src/main.cpp)

# Test executable
add_executable(RayTracerChallengeTests tests/test_main.cpp)
target_link_libraries(RayTracerChallengeTests PRIVATE Catch2::Catch2WithMain)
add_test(NAME RayTracerChallengeTests COMMAND RayTracerChallengeTests)