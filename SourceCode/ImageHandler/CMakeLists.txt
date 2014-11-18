add_library(ImageFourNeighborType ImageFourNeighborType.cpp)
add_library(ImageReader ImageReader.cpp)
target_link_libraries(ImageReader jpeg png)
