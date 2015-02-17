#include <iostream>
#include <algorithm>

#include "gtest/gtest.h"

#include "ImageFourNeighborType.h"

TEST(ImageTest, checkCoordinates) {

    vector<unsigned short> dummyPixelArray = { 0,0,0,
                                               0,0,0,
                                               0,0,0 };
    ImageFourNeighborType image(dummyPixelArray,3,3);

    unsigned int raw = 0, col = 0;

    image.getCoordinates(0,raw,col);
    ASSERT_EQ(0,raw);
    ASSERT_EQ(0,col);

    image.getCoordinates(1,raw,col);
    ASSERT_EQ(0,raw);
    ASSERT_EQ(1,col);

    image.getCoordinates(2,raw,col);
    ASSERT_EQ(0,raw);
    ASSERT_EQ(2,col);

    image.getCoordinates(3,raw,col);
    ASSERT_EQ(1,raw);
    ASSERT_EQ(0,col);

    image.getCoordinates(4,raw,col);
    ASSERT_EQ(1,raw);
    ASSERT_EQ(1,col);

    image.getCoordinates(5,raw,col);
    ASSERT_EQ(1,raw);
    ASSERT_EQ(2,col);

    image.getCoordinates(6,raw,col);
    ASSERT_EQ(2,raw);
    ASSERT_EQ(0,col);

    image.getCoordinates(7,raw,col);
    ASSERT_EQ(2,raw);
    ASSERT_EQ(1,col);

    image.getCoordinates(8,raw,col);
    ASSERT_EQ(2,raw);
    ASSERT_EQ(2,col);

}

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
