include_directories(${CMAKE_SOURCE_DIR}/ComponentTree)
include_directories(${CMAKE_SOURCE_DIR}/ImageHandler)

link_directories(${CMAKE_BINARY_DIR}/ImageHandler)
link_directories(${CMAKE_BINARY_DIR}/ComponentTree)

add_executable(TopologicalWatershed TopologicalWatershed.cpp)
target_link_libraries(TopologicalWatershed Image ComponentTree)
