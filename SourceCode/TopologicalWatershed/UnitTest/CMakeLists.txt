add_executable(TopologicalWatershedUnitTests TopologicalWatershedUnitTests.cpp)
target_link_libraries(TopologicalWatershedUnitTests gtest gtest_main pthread TopologicalWatershed)

add_test(NAME doTopologicalWatershed COMMAND TopologicalWatershedUnitTests)
