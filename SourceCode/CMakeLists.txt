cmake_minimum_required(VERSION 2.8.7)
project(TopologicalWatershed)

# version number
set(TopologicalWatershed_VERSION_MAJOR 1)
set(TopologicalWatershed_VERSION_MINOR 0)

# Options. Turn ON to build tests.
option(test "Tests." ON)

# compilation flags
SET(CMAKE_CXX_FLAGS  "-Wall -Wextra -ansi -pedantic -Weffc++ -g -std=c++11")

# headers
include_directories(${CMAKE_SOURCE_DIR}/DisjointSetCollection)
include_directories(${CMAKE_SOURCE_DIR}/ImageHandler)
include_directories(${CMAKE_SOURCE_DIR}/ComponentTree)

# include the subdirectory containing our libs
add_subdirectory(ImageHandler)
add_subdirectory(DisjointSetCollection)
add_subdirectory(ComponentTree)
add_subdirectory(TopologicalWatershed)

if (test)
    add_subdirectory(${CMAKE_SOURCE_DIR}/gtest-1.7.0)
    enable_testing()
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

    add_subdirectory(${CMAKE_SOURCE_DIR}/ImageHandler/UnitTest)
    add_subdirectory(${CMAKE_SOURCE_DIR}/DisjointSetCollection/UnitTest)
endif()
