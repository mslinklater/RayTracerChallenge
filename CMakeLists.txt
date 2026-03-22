cmake_minimum_required(VERSION 3.16)
project(RayTracerChallenge CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_COVERAGE "Build with coverage instrumentation" OFF)

find_package(Catch2 3 REQUIRED)

include(Catch)  # This provides catch_discover_tests()
enable_testing()

set(SRC_FILES   src/utils.cpp 
                src/camera.cpp
                src/color.cpp
                src/matrix.cpp
                src/sphere.cpp
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

# Main executable
add_executable(RayTracerChallenge   main.cpp ${SRC_FILES})
target_include_directories(RayTracerChallenge PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_options(RayTracerChallenge PRIVATE ${PROJECT_COVERAGE_FLAGS})
target_link_options(RayTracerChallenge PRIVATE ${PROJECT_COVERAGE_FLAGS})

# Chapter 4 challenge executable
add_executable(Chapter4Challenge chapter4.cpp ${SRC_FILES})
target_include_directories(Chapter4Challenge PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_options(Chapter4Challenge PRIVATE ${PROJECT_COVERAGE_FLAGS})
target_link_options(Chapter4Challenge PRIVATE ${PROJECT_COVERAGE_FLAGS})

# Chapter 5 challenge executable
add_executable(Chapter5Challenge chapter5.cpp ${SRC_FILES})
target_include_directories(Chapter5Challenge PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_options(Chapter5Challenge PRIVATE ${PROJECT_COVERAGE_FLAGS})
target_link_options(Chapter5Challenge PRIVATE ${PROJECT_COVERAGE_FLAGS})

# Chapter 6 challenge executable
add_executable(Chapter6Challenge chapter6.cpp ${SRC_FILES})
target_include_directories(Chapter6Challenge PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_options(Chapter6Challenge PRIVATE ${PROJECT_COVERAGE_FLAGS})
target_link_options(Chapter6Challenge PRIVATE ${PROJECT_COVERAGE_FLAGS})

# Chapter 7 challenge executable
add_executable(Chapter7Challenge chapter7.cpp ${SRC_FILES})
target_include_directories(Chapter7Challenge PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_options(Chapter7Challenge PRIVATE ${PROJECT_COVERAGE_FLAGS})
target_link_options(Chapter7Challenge PRIVATE ${PROJECT_COVERAGE_FLAGS})

# Chapter 8 challenge executable
add_executable(Chapter8Challenge chapter8.cpp ${SRC_FILES})
target_include_directories(Chapter8Challenge PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_options(Chapter8Challenge PRIVATE ${PROJECT_COVERAGE_FLAGS})
target_link_options(Chapter8Challenge PRIVATE ${PROJECT_COVERAGE_FLAGS})

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
                tests/test_lights.cpp
                tests/test_materials.cpp
                tests/test_matrix.cpp
                tests/test_transformations.cpp
                tests/test_canvas.cpp)

add_executable(RayTracerChallengeTests ${TEST_SRC} ${SRC_FILES})
target_include_directories(RayTracerChallengeTests PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(RayTracerChallengeTests PRIVATE Catch2::Catch2WithMain)
target_compile_options(RayTracerChallengeTests PRIVATE ${PROJECT_COVERAGE_FLAGS})
target_link_options(RayTracerChallengeTests PRIVATE ${PROJECT_COVERAGE_FLAGS})

# Use Catch2's test discovery instead of manual add_test
catch_discover_tests(RayTracerChallengeTests)

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