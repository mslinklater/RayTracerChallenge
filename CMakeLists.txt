cmake_minimum_required(VERSION 3.16)
project(RayTracerChallenge CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_COVERAGE "Build with coverage instrumentation" OFF)

include(FetchContent)

find_package(Catch2 3 REQUIRED)

set(JSONCPP_WITH_TESTS OFF CACHE BOOL "" FORCE)
set(JSONCPP_WITH_POST_BUILD_UNITTEST OFF CACHE BOOL "" FORCE)
set(JSONCPP_WITH_WARNING_AS_ERROR OFF CACHE BOOL "" FORCE)
set(JSONCPP_WITH_EXAMPLE OFF CACHE BOOL "" FORCE)
set(JSONCPP_WITH_CMAKE_PACKAGE OFF CACHE BOOL "" FORCE)
set(JSONCPP_WITH_PKGCONFIG_SUPPORT OFF CACHE BOOL "" FORCE)
set(JSONCPP_WITH_INSTALL OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE)
set(BUILD_OBJECT_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    jsoncpp
    GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp.git
    GIT_TAG 1.9.7
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(jsoncpp)

include(Catch)  # This provides catch_discover_tests()
enable_testing()

set(SRC_FILES   src/utils.cpp
                src/camera.cpp
                src/color.cpp
                src/matrix.cpp
                src/shapes/plane.cpp
                src/shapes/shape.cpp
                src/shapes/sphere.cpp
                src/shapes/cube.cpp
                src/pattern.cpp
                src/ray.cpp
                src/light.cpp
                src/tuple.cpp
                src/intersection.cpp
                src/material.cpp
                src/renderer.cpp
                src/world.cpp
                src/canvas.cpp)

set(PROJECT_COVERAGE_FLAGS "")

if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        # Required for gcov/llvm-cov compatible coverage artifacts.
        set(PROJECT_COVERAGE_FLAGS -O0 -g --coverage)
    else()
        message(WARNING "Coverage requested, but compiler '${CMAKE_CXX_COMPILER_ID}' is not supported by this setup.")
    endif()
endif()

add_library(RayTracerChallengeLib STATIC ${SRC_FILES})
target_include_directories(RayTracerChallengeLib PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_options(RayTracerChallengeLib PRIVATE ${PROJECT_COVERAGE_FLAGS})
target_link_libraries(RayTracerChallengeLib PUBLIC jsoncpp_static)

function(add_raytracer_executable target_name source_file)
    add_executable(${target_name} ${source_file})
    target_link_libraries(${target_name} PRIVATE RayTracerChallengeLib)
    target_link_options(${target_name} PRIVATE ${PROJECT_COVERAGE_FLAGS})
endfunction()

# Main executable
add_raytracer_executable(RayTracerChallenge main.cpp)

# Chapter 4 challenge executable
add_raytracer_executable(Chapter4Challenge chapter4.cpp)

# Chapter 5 challenge executable
add_raytracer_executable(Chapter5Challenge chapter5.cpp)

# Chapter 6 challenge executable
add_raytracer_executable(Chapter6Challenge chapter6.cpp)

# Chapter 7 challenge executable
add_raytracer_executable(Chapter7Challenge chapter7.cpp)

# Chapter 8 challenge executable
add_raytracer_executable(Chapter8Challenge chapter8.cpp)

# Chapter 9 challenge executable
add_raytracer_executable(Chapter9Challenge chapter9.cpp)

# Chapter 10 challenge executable
add_raytracer_executable(Chapter10Challenge chapter10.cpp)

# Chapter 11 challenge executable
add_raytracer_executable(Chapter11Challenge chapter11.cpp)

# Chapter 12 challenge executable
add_raytracer_executable(Chapter12Challenge chapter12.cpp)

# Test executable
set(TEST_SRC    tests/test_main.cpp
                tests/test_camera.cpp
                tests/test_color.cpp
                tests/test_tuples.cpp
                tests/test_utils.cpp
                tests/test_rays.cpp
                tests/test_world.cpp
                tests/test_intersections.cpp
                tests/test_spheres.cpp
                tests/test_planes.cpp
                tests/test_lights.cpp
                tests/test_lighting.cpp
                tests/test_shapes.cpp
                tests/test_materials.cpp
                tests/test_matrix.cpp
                tests/test_cubes.cpp
                tests/test_json.cpp
                tests/test_patterns.cpp
                tests/test_transformations.cpp
                tests/test_determinism.cpp
                tests/test_canvas.cpp)

add_executable(RayTracerChallengeTests ${TEST_SRC})
target_link_libraries(RayTracerChallengeTests PRIVATE RayTracerChallengeLib Catch2::Catch2WithMain)
target_link_options(RayTracerChallengeTests PRIVATE ${PROJECT_COVERAGE_FLAGS})

# Use Catch2's test discovery instead of manual add_test
catch_discover_tests(RayTracerChallengeTests)

# Doxygen documentation target
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/docs)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_LATEX NO)
    set(DOXYGEN_RECURSIVE YES)
    set(DOXYGEN_EXTRACT_ALL YES)
    doxygen_add_docs(doxygen
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
        COMMENT "Generating HTML documentation in docs/"
    )
else()
    message(STATUS "Doxygen not found -- 'doxygen' target will not be available.")
endif()

if(ENABLE_COVERAGE)
    find_program(LCOV_BIN lcov)
    find_program(GENHTML_BIN genhtml)
    if(LCOV_BIN)
        add_custom_target(coverage
            COMMAND ${LCOV_BIN} --zerocounters --directory ${CMAKE_BINARY_DIR}
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
            COMMAND ${LCOV_BIN} --capture --ignore-errors inconsistent,unsupported,format --directory ${CMAKE_BINARY_DIR} --output-file ${CMAKE_BINARY_DIR}/coverage_raw.info
            COMMAND ${LCOV_BIN} --extract ${CMAKE_BINARY_DIR}/coverage_raw.info "${CMAKE_SOURCE_DIR}/src/*" "${CMAKE_SOURCE_DIR}/include/*" --ignore-errors inconsistent,unused --output-file ${CMAKE_BINARY_DIR}/coverage.info
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            DEPENDS RayTracerChallengeTests
            COMMENT "Running tests and generating ${CMAKE_BINARY_DIR}/coverage.info"
            VERBATIM
        )

        if(GENHTML_BIN)
            add_custom_target(coverage-html
                COMMAND ${GENHTML_BIN} ${CMAKE_BINARY_DIR}/coverage.info --ignore-errors category,inconsistent --output-directory ${CMAKE_BINARY_DIR}/html
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                DEPENDS coverage
                COMMENT "Generating HTML coverage report in ${CMAKE_BINARY_DIR}/html"
                VERBATIM
            )
        else()
            message(WARNING "ENABLE_COVERAGE is ON but 'genhtml' was not found. Install lcov to use the coverage-html target.")
        endif()
    else()
        message(WARNING "ENABLE_COVERAGE is ON but 'lcov' was not found. Install lcov to use the coverage target.")
    endif()
endif()
