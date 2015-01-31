#include <iostream>
#include "gtest/gtest.h"
#include "ImageFourNeighborType.h"
#include "TopologicalWatershed.h"

/*****************************************************************************
  NAIVE TOPOLOGICAL WATERSGED TESTS
******************************************************************************/

TEST(NaiveTopologicalWatershed, performTopologicalWatershedOnSynteticImage_1)
{
    vector<ushort> pixels = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                              0,1,2,11,11,11,4,4,3,3,0,5,11,11,11,3,2,0,
                              0,3,12,6,7,5,11,7,6,5,7,11,5,7,7,12,4,0,
                              0,15,7,4,2,3,5,11,11,11,11,5,3,1,7,7,15,0,
                              0,3,14,7,3,5,11,8,7,8,8,11,5,3,7,14,5,0,
                              0,1,2,13,11,11,3,2,2,1,2,4,11,11,11,4,3,0,
                              0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

    ImageFourNeighborType image(pixels,18,7);
    doNaiveTopologicalWatershed(image);

    vector<ushort> expectedResult = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                      0,0,0,11,11,11,0,0,0,0,0,0,11,11,11,0,0,0,
                                      0,0,11,2,2,2,11,0,0,0,0,11,1,1,1,11,0,0,
                                      0,11,2,2,2,2,2,11,0,0,11,1,1,1,1,1,11,0,
                                      0,0,11,2,2,2,11,0,0,0,0,11,1,1,1,11,0,0,
                                      0,0,0,11,11,11,0,0,0,0,0,0,11,11,11,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

    ASSERT_EQ(expectedResult,image.getPixels());
}

TEST(NaiveTopologicalWatershed, performTopologicalWatershedOnSynteticImage_2)
{
    vector<ushort> pixels = { 1,3,2,3,
                              4,3,2,1,
                              4,4,4,4,
                              1,2,3,4 };

    ImageFourNeighborType image(pixels,4,4);
    doNaiveTopologicalWatershed(image);

    vector<ushort> expectedResult = { 1,3,1,1,
                                      3,1,1,1,
                                      4,4,4,1,
                                      1,1,1,4 };

    ASSERT_EQ(expectedResult,image.getPixels());

}

TEST(NaiveTopologicalWatershed, performTopologicalWatershedOnSynteticImage_3)
{
    vector<ushort> pixels = { 1,1,1,1,1,
                              1,1,1,1,1,
                              1,1,200,1,1,
                              1,1,1,1,1,
                              1,1,1,1,1 };

    ImageFourNeighborType image(pixels,5,5);
    doNaiveTopologicalWatershed(image);

    vector<ushort> expectedResult = { 1,1,1,1,1,
                                      1,1,1,1,1,
                                      1,1,1,1,1,
                                      1,1,1,1,1,
                                      1,1,1,1,1 };

    ASSERT_EQ(expectedResult,image.getPixels());

}

TEST(NaiveTopologicalWatershed, performTopologicalWatershedOnSynteticImage_4)
{
    vector<ushort> pixels = { 3,3,3,3,3,
                              3,3,3,3,3,
                              3,3,1,3,3,
                              3,3,3,3,3,
                              3,3,3,3,3 };

    ImageFourNeighborType image(pixels,5,5);
    doNaiveTopologicalWatershed(image);

    vector<ushort> expectedResult = { 1,1,1,1,1,
                                      1,1,1,1,1,
                                      1,1,1,1,1,
                                      1,1,1,1,1,
                                      1,1,1,1,1 };

    ASSERT_EQ(expectedResult,image.getPixels());

}

TEST(NaiveTopologicalWatershed, performTopologicalWatershedOnSynteticImage_5)
{
    vector<ushort> pixels = { 1,1,1,1,3,0,0,0,0,
                              0,0,0,3,2,3,0,0,0,
                              0,0,3,2,2,2,3,0,0,
                              0,3,1,3,2,3,1,3,0,
                              0,0,3,0,2,0,2,0,0,
                              0,0,0,0,2,0,0,0,0 };

    ImageFourNeighborType image(pixels,9,6);
    doNaiveTopologicalWatershed(image);

    vector<ushort> expectedResult = { 0,0,0,0,2,0,0,0,0,
                                      0,0,0,0,0,2,0,0,0,
                                      0,0,3,0,0,0,2,0,0,
                                      0,3,1,3,0,2,1,2,0,
                                      0,0,3,0,2,0,2,0,0,
                                      0,0,0,0,2,0,0,0,0 };

    ASSERT_EQ(expectedResult,image.getPixels());

}

TEST(NaiveTopologicalWatershed, performTopologicalWatershedOnSynteticImage_6)
{
    vector<ushort> pixels = { 0,9,0,0,0,8,8,2,2,2,5,3,3,
                              0,9,6,5,8,8,8,8,2,7,7,7,3,
                              0,9,1,1,8,9,8,8,2,7,7,3,3,
                              0,9,9,7,8,8,8,8,2,7,7,7,3,
                              0,9,0,0,0,8,8,5,2,7,7,5,3,
                              0,9,9,0,0,8,8,5,2,7,7,5,3 };

    ImageFourNeighborType image(pixels,13,6);
    doNaiveTopologicalWatershed(image);

    vector<ushort> expectedResult = { 0,9,0,0,0,0,8,2,2,2,5,3,3,
                                      0,9,5,5,0,0,0,8,2,2,2,5,3,
                                      0,9,1,1,5,0,0,8,2,2,5,3,3,
                                      0,0,9,7,7,7,0,8,2,2,2,5,3,
                                      0,9,0,0,0,0,8,2,2,2,5,3,3,
                                      0,0,9,0,0,0,8,2,2,2,5,3,3 };

    ASSERT_EQ(expectedResult,image.getPixels());
}

/*****************************************************************************
  LINEAL TOPOLOGICAL WATERSGED TESTS
******************************************************************************/

TEST(LinearTopologicalWatershed, performTopologicalWatershedOnSynteticImage_1)
{
    vector<ushort> pixels = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                              0,1,2,11,11,11,4,4,3,3,0,5,11,11,11,3,2,0,
                              0,3,12,6,7,5,11,7,6,5,7,11,5,7,7,12,4,0,
                              0,15,7,4,2,3,5,11,11,11,11,5,3,1,7,7,15,0,
                              0,3,14,7,3,5,11,8,7,8,8,11,5,3,7,14,5,0,
                              0,1,2,13,11,11,3,2,2,1,2,4,11,11,11,4,3,0,
                              0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

    ImageFourNeighborType image(pixels,18,7);
    ComponentTree tree(image);
    doLinearTopologicalWatershed(image, tree);

    vector<ushort> expectedResult = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                      0,0,0,11,11,11,0,0,0,0,0,0,11,11,11,0,0,0,
                                      0,0,11,2,2,2,11,0,0,0,0,11,1,1,1,11,0,0,
                                      0,11,2,2,2,2,2,11,0,0,11,1,1,1,1,1,11,0,
                                      0,0,11,2,2,2,11,0,0,0,0,11,1,1,1,11,0,0,
                                      0,0,0,11,11,11,0,0,0,0,0,0,11,11,11,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

    ASSERT_EQ(expectedResult,image.getPixels());
}

TEST(LinearTopologicalWatershed, performTopologicalWatershedOnSynteticImage_2)
{
    vector<ushort> pixels = { 1,3,2,3,
                              4,3,2,1,
                              4,4,4,4,
                              1,2,3,4 };

    ImageFourNeighborType image(pixels,4,4);
    ComponentTree tree(image);
    doLinearTopologicalWatershed(image, tree);

    vector<ushort> expectedResult = { 1,3,1,1,
                                      1,3,1,1,
                                      4,4,4,1,
                                      1,1,1,4 };

    ASSERT_EQ(expectedResult,image.getPixels());

}

TEST(LinearTopologicalWatershed, performTopologicalWatershedOnSynteticImage_3)
{
    vector<ushort> pixels = { 1,1,1,1,1,
                              1,1,1,1,1,
                              1,1,200,1,1,
                              1,1,1,1,1,
                              1,1,1,1,1 };

    ImageFourNeighborType image(pixels,5,5);
    ComponentTree tree(image);
    doLinearTopologicalWatershed(image, tree);

    vector<ushort> expectedResult = { 1,1,1,1,1,
                                      1,1,1,1,1,
                                      1,1,1,1,1,
                                      1,1,1,1,1,
                                      1,1,1,1,1 };

    ASSERT_EQ(expectedResult,image.getPixels());

}

TEST(LinearTopologicalWatershed, performTopologicalWatershedOnSynteticImage_4)
{
    vector<ushort> pixels = { 3,3,3,3,3,
                              3,3,3,3,3,
                              3,3,1,3,3,
                              3,3,3,3,3,
                              3,3,3,3,3 };

    ImageFourNeighborType image(pixels,5,5);
    ComponentTree tree(image);
    doLinearTopologicalWatershed(image, tree);

    vector<ushort> expectedResult = { 1,1,1,1,1,
                                      1,1,1,1,1,
                                      1,1,1,1,1,
                                      1,1,1,1,1,
                                      1,1,1,1,1 };

    ASSERT_EQ(expectedResult,image.getPixels());

}

TEST(LinearTopologicalWatershed, performTopologicalWatershedOnSynteticImage_5)
{
    vector<ushort> pixels = { 1,1,1,1,3,0,0,0,0,
                              0,0,0,3,2,3,0,0,0,
                              0,0,3,2,2,2,3,0,0,
                              0,3,1,3,2,3,1,3,0,
                              0,0,3,0,2,0,2,0,0,
                              0,0,0,0,2,0,0,0,0 };

    ImageFourNeighborType image(pixels,9,6);
    ComponentTree tree(image);
    doLinearTopologicalWatershed(image, tree);

    vector<ushort> expectedResult = { 0,0,0,0,2,0,0,0,0,
                                      0,0,0,0,2,0,0,0,0,
                                      0,0,3,0,2,0,2,0,0,
                                      0,3,1,3,2,2,1,2,0,
                                      0,0,3,0,2,0,2,0,0,
                                      0,0,0,0,2,0,0,0,0 };

    ASSERT_EQ(expectedResult,image.getPixels());

}

TEST(LinearTopologicalWatershed, performTopologicalWatershedOnSynteticImage_6)
{
    vector<ushort> pixels = { 0,9,0,0,0,8,8,2,2,2,5,3,3,
                              0,9,6,5,8,8,8,8,2,7,7,7,3,
                              0,9,1,1,8,9,8,8,2,7,7,3,3,
                              0,9,9,7,8,8,8,8,2,7,7,7,3,
                              0,9,0,0,0,8,8,5,2,7,7,5,3,
                              0,9,9,0,0,8,8,5,2,7,7,5,3 };

    ImageFourNeighborType image(pixels,13,6);
    ComponentTree tree(image);
    doLinearTopologicalWatershed(image, tree);

    image.printMe();

    vector<ushort> expectedResult = { 0,9,0,0,0,0,8,2,2,2,5,3,3,
                                      0,9,5,5,0,0,0,8,2,2,2,5,3,
                                      0,9,1,1,7,0,0,8,2,2,5,3,3,
                                      0,0,9,7,0,7,8,2,2,2,5,3,3,
                                      0,9,0,0,0,0,8,2,2,2,2,5,3,
                                      0,0,9,0,0,0,8,2,2,2,2,5,3 };

    ASSERT_EQ(expectedResult,image.getPixels());

}

TEST(LinearTopologicalWatershed, performTopologicalWatershedOnSynteticImage_7)
{
    vector<ushort> pixels = { 255,255,255,255,255,
                              255,63,61,59,58,
                              255,58,57,56,56,
                              255,57,58,57,56,
                              255,56,57,57,56 };

    ImageFourNeighborType image(pixels,5,5);
    ComponentTree tree(image);

    doLinearTopologicalWatershed(image, tree);

    vector<ushort> expectedResult = { 56,56,56,56,56,
                                      56,56,56,56,56,
                                      57,56,56,56,56,
                                      56,57,56,56,56,
                                      56,56,57,56,56 };

    ASSERT_EQ(expectedResult,image.getPixels());

}


/*****************************************************************************
  PARALLEL TOPOLOGICAL WATERSGED TESTS
******************************************************************************/

TEST(ParallelTopologicalWatershed, performParallelTopologicalWatershedOnSynteticImage)
{
    vector<ushort> pixels = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                              0,1,2,11,11,11,4,4,3,3,0,5,11,11,11,3,2,0,
                              0,3,12,6,7,5,11,7,6,5,7,11,5,7,7,12,4,0,
                              0,15,7,4,2,3,5,11,11,11,11,5,3,1,7,7,15,0,
                              0,3,14,7,3,5,11,8,7,8,8,11,5,3,7,14,5,0,
                              0,1,2,13,11,11,3,2,2,1,2,4,11,11,11,4,3,0,
                              0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

    ImageFourNeighborType image(pixels,18,7);
    ComponentTree tree(image);
    doParallelTopologicalWatershed(image, tree, 4);

    vector<ushort> expectedResult = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                      0,0,0,11,11,11,0,0,0,0,0,0,11,11,11,0,0,0,
                                      0,0,11,2,2,2,11,0,0,0,0,11,1,1,1,11,0,0,
                                      0,11,2,2,2,2,2,11,0,0,11,1,1,1,1,1,11,0,
                                      0,0,11,2,2,2,11,0,0,0,0,11,1,1,1,11,0,0,
                                      0,0,0,11,11,11,0,0,0,0,0,0,11,11,11,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

    ASSERT_EQ(expectedResult,image.getPixels());
}
