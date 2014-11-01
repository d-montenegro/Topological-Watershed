#include <iostream>
#include "gtest/gtest.h"
#include "ImageFourNeighborType.h"
#include "LinearTopologicalWatershed.h"

TEST(LinearTopologicalWatershed, performTopologicalWatershedOnSynteticImage) {
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

TEST(ParallelTopologicalWatershed, performParallelTopologicalWatershedOnSynteticImage) {
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
