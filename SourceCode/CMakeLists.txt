cmake_minimum_required(VERSION 2.8.7)
project(TopologicalWatershed)

# version number
set(TopologicalWatershed_VERSION_MAJOR 1)
set(TopologicalWatershed_VERSION_MINOR 0)

find_library(png++ REQUIRED)

# Options. Turn ON to build tests.
option(UNITTEST "Tests." ON)

# compilation flags
SET(CMAKE_CXX_FLAGS_DEBUG  "-Wall -Wextra -ansi -pedantic -Weffc++ -gdwarf-3 -std=c++11 -Wl,--no-as-needed -pthread")
SET(CMAKE_CXX_FLAGS_RELEASE  "-O3 -std=c++11 -Wl,--no-as-needed -pthread")

# headers
include_directories(${CMAKE_SOURCE_DIR}/DisjointSetCollection)
include_directories(${CMAKE_SOURCE_DIR}/ImageHandler)
include_directories(${CMAKE_SOURCE_DIR}/ComponentTree)
include_directories(${CMAKE_SOURCE_DIR}/TopologicalWatershed)
include_directories(${CMAKE_SOURCE_DIR}/BidimiensionalArrayPartitioner)

# include the subdirectory containing our libs
add_subdirectory(ImageHandler)
add_subdirectory(DisjointSetCollection)
add_subdirectory(ComponentTree)
add_subdirectory(TopologicalWatershed)
add_subdirectory(BidimiensionalArrayPartitioner)

if(UNITTEST)
    enable_testing()
    find_package(GTest REQUIRED)
    add_subdirectory(${CMAKE_SOURCE_DIR}/ImageHandler/UnitTest)
    add_subdirectory(${CMAKE_SOURCE_DIR}/DisjointSetCollection/UnitTest)
    add_subdirectory(${CMAKE_SOURCE_DIR}/ComponentTree/UnitTest)
    add_subdirectory(${CMAKE_SOURCE_DIR}/BidimiensionalArrayPartitioner/UnitTest)
    add_subdirectory(${CMAKE_SOURCE_DIR}/TopologicalWatershed/UnitTest)
endif(UNITTEST)
