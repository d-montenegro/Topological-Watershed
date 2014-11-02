#include "gtest/gtest.h"
#include "BidimiensionalArrayPartitioner.h"

using namespace std;

TEST(BidimiensionalArrayPartitioner, test7x7in4) {

    /* we have a 7 x 7 array to divide into 4 tiles, so here are the
     * positions and the expected division:
     *
     *  00,01,02 - 03,04,05,06
     *  07,08,09 - 10,11,12,13
     *  14,15,16 - 17,18,19,20
     *  -- -- -- - -- -- -- --
     *  21,22,23 - 24,25,26,27
     *  28,29,30 - 31,32,33,34
     *  35,36,37 - 38,39,40,41
     *  42,43,44 - 45,46,47,48
     */

    vector<Tile> tiles = divideSquareIntoTiles(7,7,4);

    Tile expectedTile0 = { 0,1,2,
                           7,8,9,
                           14,15,16 };

    Tile expectedTile1 = { 3,4,5,6,
                           10,11,12,13,
                           17,18,19,20 };

    Tile expectedTile2 = { 21,22,23,
                           28,29,30,
                           35,36,37,
                           42,43,44 };

    Tile expectedTile3 = { 24,25,26,27,
                           31,32,33,34,
                           38,39,40,41,
                           45,46,47,48 };

    ASSERT_EQ(4, tiles.size());
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

    ASSERT_EQ(9, tiles.size());
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

TEST(BidimiensionalArrayPartitioner, test18x7in4) {

    /* we have a 7 x 7 array to divide into 4 tiles, so here are the
     * positions and the expected division:
     *
     * 00 01 02 03 04 05 06 07 08 - 09 10 11 12 13 14 15 16 17
     * 18 19 20 21 22 23 24 25 26 - 27 28 29 30 31 32 33 34 35
     * 36 37 38 39 40 41 42 43 44 - 45 46 47 48 49 50 51 52 53
     * -- -- -- -- -- -- -- -- -- - -- -- -- -- -- -- -- -- --
     * 54 55 56 57 58 59 60 61 62 - 63 64 65 66 67 68 69 70 71
     * 72 73 74 75 76 77 78 79 80 - 81 82 83 84 85 86 87 88 89
     * 90 91 92 93 94 95 96 97 98 - 99 100 101 102 103 104 105 106 107
     * 108 109 110 111 112 113 114 115 116 - 117 118 119 120 121 122 123 124 125
     */

    vector<Tile> tiles = divideSquareIntoTiles(18,7,4);

    Tile expectedTile0 = { 0, 1, 2, 3, 4, 5, 6, 7, 8,
                           18, 19, 20, 21, 22, 23, 24, 25, 26,
                           36, 37, 38, 39, 40, 41, 42, 43, 44 };

    Tile expectedTile1 = { 9, 10, 11, 12, 13, 14, 15, 16, 17,
                           27, 28, 29, 30, 31, 32, 33, 34, 35,
                           45, 46, 47, 48, 49, 50, 51, 52, 53 };

    Tile expectedTile2 = { 54, 55, 56, 57, 58, 59, 60, 61, 62,
                           72, 73, 74, 75, 76, 77, 78, 79, 80,
                           90, 91, 92, 93, 94, 95, 96, 97, 98,
                           108, 109, 110, 111, 112, 113, 114, 115, 116 };

    Tile expectedTile3 = { 63, 64, 65, 66, 67, 68, 69, 70, 71,
                           81, 82, 83, 84, 85, 86, 87, 88, 89,
                           99, 100, 101, 102, 103, 104, 105, 106, 107,
                           117, 118, 119, 120, 121, 122, 123, 124, 125 };

    ASSERT_EQ(4, tiles.size());
    ASSERT_EQ(expectedTile0, tiles.at(0));
    ASSERT_EQ(expectedTile1, tiles.at(1));
    ASSERT_EQ(expectedTile2, tiles.at(2));
    ASSERT_EQ(expectedTile3, tiles.at(3));
}

TEST(BidimiensionalArrayPartitioner, test18x7in9) {

    /* we have a 7 x 7 array to divide into 4 tiles, so here are the
     * positions and the expected division:
     *
     * 00 01 02 03 04 05 - 06 07 08 09 10 11 - 12 13 14 15 16 17
     * 18 19 20 21 22 23 - 24 25 26 27 28 29 - 30 31 32 33 34 35
     * -- -- -- -- -- -- -- -- -- - -- -- -- -- -- -- -- -- --
     * 36 37 38 39 40 41 - 42 43 44 45 46 47 - 48 49 50 51 52 53
     * 54 55 56 57 58 59 - 60 61 62 63 64 65 - 66 67 68 69 70 71
     * -- -- -- -- -- -- -- -- -- - -- -- -- -- -- -- -- -- --
     * 72 73 74 75 76 77 - 78 79 80 81 82 83 - 84 85 86 87 88 89
     * 90 91 92 93 94 95 - 96 97 98 99 100 101 - 102 103 104 105 106 107
     * 108 109 110 111 112 113 - 114 115 116 117 118 119 - 120 121 122 123 124 125
     */

    vector<Tile> tiles = divideSquareIntoTiles(18,7,9);

    Tile expectedTile0 = { 0, 1, 2, 3, 4, 5,
                           18, 19, 20, 21, 22, 23 };

    Tile expectedTile1 = { 6, 7, 8, 9, 10, 11,
                           24, 25, 26, 27, 28, 29 };

    Tile expectedTile2 = { 12, 13, 14, 15, 16, 17,
                           30, 31, 32, 33, 34, 35 };

    Tile expectedTile3 = { 36, 37, 38, 39, 40, 41,
                           54, 55, 56, 57, 58, 59 };

    Tile expectedTile4 = { 42, 43, 44, 45, 46, 47,
                           60, 61, 62, 63, 64, 65 };

    Tile expectedTile5 = { 48, 49, 50, 51, 52, 53,
                           66, 67, 68, 69, 70, 71 };

    Tile expectedTile6 = { 72, 73, 74, 75, 76, 77,
                           90, 91, 92, 93, 94, 95,
                           108, 109, 110, 111, 112, 113 };

    Tile expectedTile7 = { 78, 79, 80, 81, 82, 83,
                           96, 97, 98, 99, 100, 101,
                           114, 115, 116, 117, 118, 119 };

    Tile expectedTile8 = { 84, 85, 86, 87, 88, 89,
                           102, 103, 104, 105, 106, 107,
                           120, 121, 122, 123, 124, 125 };

    ASSERT_EQ(9, tiles.size());
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
