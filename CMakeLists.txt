cmake_minimum_required(VERSION 3.16)
project(RayTracerChallenge CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Catch2 3 REQUIRED)

enable_testing()

set(SRC_FILES   src/utils.cpp 
                src/matrix.cpp
                src/canvas.cpp)

# Main executable
add_executable(RayTracerChallenge   src/main.cpp ${SRC_FILES})
target_include_directories(RayTracerChallenge PRIVATE ${PROJECT_SOURCE_DIR}/src)

# Test executable
set(TEST_SRC    tests/test_main.cpp 
                tests/test_color.cpp 
                tests/test_tuples.cpp 
                tests/test_utils.cpp
                tests/test_matrix.cpp
                tests/test_canvas.cpp)

add_executable(RayTracerChallengeTests ${TEST_SRC} ${SRC_FILES})
target_include_directories(RayTracerChallengeTests PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(RayTracerChallengeTests PRIVATE Catch2::Catch2WithMain)
add_test(NAME RayTracerChallengeTests COMMAND RayTracerChallengeTests)