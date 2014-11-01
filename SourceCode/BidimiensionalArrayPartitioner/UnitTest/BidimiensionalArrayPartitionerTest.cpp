#include "gtest/gtest.h"
#include "BidimiensionalArrayPartitioner.h"

using namespace std;

TEST(BidimiensionalArrayPartitioner, test7x7in4) {

    /* we have a 7 x 7 array to divide into 4 tiles, so here are the
     * positions and the expected division:
     *
     *  00,01,02,03 - 04,05,06
     *  07,08,09,10 - 11,12,13
     *  14,15,16,17 - 18,19,20
     *  21,22,23,24 - 25,26,27
     *  -- -- -- -- - -- -- --
     *  28,29,30,31 - 32,33,34
     *  35,36,37,38 - 39,40,41
     *  42,43,44,45 - 46,47,48
     */

    vector<Tile> tiles = divideSquareIntoTiles(7,7,4);

    Tile expectedTile0 = { 0,1,2,3,
                           7,8,9,10,
                           14,15,16,17,
                           21,22,23,24 };

    Tile expectedTile1 = { 4,5,6,
                           11,12,13,
                           18,19,20,
                           25,26,27 };

    Tile expectedTile2 = { 28,29,30,31,
                           35,36,37,38,
                           42,43,44,45 };

    Tile expectedTile3 = { 32,33,34,
                           39,40,41,
                           46,47,48 };

    ASSERT_EQ(expectedTile0, tiles.at(0));
    ASSERT_EQ(expectedTile1, tiles.at(1));
    ASSERT_EQ(expectedTile2, tiles.at(2));
    ASSERT_EQ(expectedTile3, tiles.at(3));

}

TEST(BidimiensionalArrayPartitioner, test10x10in9) {

    /* we have a 7 x 7 array to divide into 4 tiles, so here are the
     * positions and the expected division:
     *
     *  00,01,02 - 03,04,05 - 06,07,08,09
     *  10,11,12 - 13,14,15 - 16,17,18,19
     *  20,21,22 - 23,24,25 - 26,27,28,29
     *  -- -- -- - -- -- -- - -- -- --
     *  30,31,32 - 33,34,35 - 36,37,38,39
     *  40,41,42 - 43,44,45 - 46,47,48,49
     *  50,51,52 - 53,54,55 - 56,57,58,59
     *  -- -- -- - -- -- -- - -- -- --
     *  60,61,62 - 63,64,65 - 66,67,68,69
     *  70,71,72 - 73,74,75 - 76,77,78,79
     *  80,81,82 - 83,84,85 - 86,87,88,89
     *  90,91,92 - 93,94,95 - 96,97,98,99
     */

    vector<Tile> tiles = divideSquareIntoTiles(10,10,9);

    Tile expectedTile0 = { 0,1,2,
                           10,11,12,
                           20,21,22 };

    Tile expectedTile1 = { 3,4,5,
                           13,14,15,
                           23,24,25 };

    Tile expectedTile2 = { 6,7,8,9,
                           16,17,18,19,
                           26,27,28,29 };

    Tile expectedTile3 = { 30,31,32,
                           40,41,42,
                           50,51,52 };

    Tile expectedTile4 = { 33,34,35,
                           43,44,45,
                           53,54,55 };

    Tile expectedTile5 = { 36,37,38,39,
                           46,47,48,49,
                           56,57,58,59 };

    Tile expectedTile6 = { 60,61,62,
                           70,71,72,
                           80,81,82,
                           90,91,92 };

    Tile expectedTile7 = { 63,64,65,
                           73,74,75,
                           83,84,85,
                           93,94,95 };

    Tile expectedTile8 = { 66,67,68,69,
                           76,77,78,79,
                           86,87,88,89,
                           96,97,98,99 };


    ASSERT_EQ(expectedTile0, tiles.at(0));
    ASSERT_EQ(expectedTile1, tiles.at(1));
    ASSERT_EQ(expectedTile2, tiles.at(2));
    ASSERT_EQ(expectedTile3, tiles.at(3));
    ASSERT_EQ(expectedTile4, tiles.at(4));
    ASSERT_EQ(expectedTile5, tiles.at(5));
    ASSERT_EQ(expectedTile6, tiles.at(6));
    ASSERT_EQ(expectedTile7, tiles.at(7));
    ASSERT_EQ(expectedTile8, tiles.at(8));

}
