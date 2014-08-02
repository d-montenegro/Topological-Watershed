add_executable(ImageUnitTests ImageUnitTests.cpp)
target_link_libraries(ImageUnitTests gtest gtest_main Image)

add_test(NAME checkNeighbors COMMAND ImageUnitTests)
