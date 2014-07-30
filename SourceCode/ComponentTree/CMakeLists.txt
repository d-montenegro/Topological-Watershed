include_directories(${CMAKE_SOURCE_DIR}/DisjointSetCollection)
include_directories(${CMAKE_SOURCE_DIR}/ImageHandler)

add_library(ComponentTree ComponentTree.cpp)
target_link_libraries(ComponentTree DisjointSetCollection Image)
