#include <iostream>
#include <algorithm>

#include "gtest/gtest.h"

#include "ImageFourNeighborType.h"


TEST(ImageTest, checkNeighbors) {
    vector<unsigned short> dummyPixelArray = { 0,0,0,0,0,0,0,0,0 };
    ImageFourNeighborType image(dummyPixelArray,3,3);

    set<unsigned int> neighbors = image.getNeighbors(4);
    ASSERT_EQ(4,neighbors.size());
    ASSERT_TRUE(std::find(neighbors.begin(), neighbors.end(), 1) != neighbors.end());
    ASSERT_TRUE(std::find(neighbors.begin(), neighbors.end(), 3) != neighbors.end());
    ASSERT_TRUE(std::find(neighbors.begin(), neighbors.end(), 5) != neighbors.end());
    ASSERT_TRUE(std::find(neighbors.begin(), neighbors.end(), 7) != neighbors.end());

    neighbors = image.getNeighbors(0);
    ASSERT_EQ(2,neighbors.size());
    ASSERT_TRUE(std::find(neighbors.begin(), neighbors.end(), 1) != neighbors.end());
    ASSERT_TRUE(std::find(neighbors.begin(), neighbors.end(), 3) != neighbors.end());

    neighbors = image.getNeighbors(2);
    ASSERT_EQ(2,neighbors.size());
    ASSERT_TRUE(std::find(neighbors.begin(), neighbors.end(), 1) != neighbors.end());
    ASSERT_TRUE(std::find(neighbors.begin(), neighbors.end(), 5) != neighbors.end());

    neighbors = image.getNeighbors(6);
    ASSERT_EQ(2,neighbors.size());
    ASSERT_TRUE(std::find(neighbors.begin(), neighbors.end(), 3) != neighbors.end());
    ASSERT_TRUE(std::find(neighbors.begin(), neighbors.end(), 7) != neighbors.end());

    neighbors = image.getNeighbors(8);
    ASSERT_EQ(2,neighbors.size());
    ASSERT_TRUE(std::find(neighbors.begin(), neighbors.end(), 5) != neighbors.end());
    ASSERT_TRUE(std::find(neighbors.begin(), neighbors.end(), 7) != neighbors.end());

}
