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

    Tile expectedTile0(0,0,2,2,{ 0,1,2,
                                 7,8,9,
                                 14,15,16 });

    Tile expectedTile1(0,3,2,6,{ 3,4,5,6,
                                 10,11,12,13,
                                 17,18,19,20 });

    Tile expectedTile2(3,0,6,2,{ 21,22,23,
                                 28,29,30,
                                 35,36,37,
                                 42,43,44 });

    Tile expectedTile3(3,3,6,6,{ 24,25,26,27,
                                 31,32,33,34,
                                 38,39,40,41,
                                 45,46,47,48 });

    ASSERT_EQ(expectedTile0, tiles.at(0));
    ASSERT_EQ(expectedTile1, tiles.at(1));
    ASSERT_EQ(expectedTile2, tiles.at(2));
    ASSERT_EQ(expectedTile3, tiles.at(3));
}

TEST(BidimiensionalArrayPartitioner, test10x10in9) {

    /* we have a 10 x 10 array to divide into 9 tiles, so here are the
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

    Tile expectedTile0(0,0,2,2,{ 0,1,2,
                                 10,11,12,
                                 20,21,22 });

    Tile expectedTile1(0,3,2,5,{ 3,4,5,
                                 13,14,15,
                                 23,24,25 });

    Tile expectedTile2(0,6,2,9,{ 6,7,8,9,
                                 16,17,18,19,
                                 26,27,28,29 });

    Tile expectedTile3(3,0,5,2, { 30,31,32,
                                  40,41,42,
                                  50,51,52 });

    Tile expectedTile4(3,3,5,5,{ 33,34,35,
                                 43,44,45,
                                 53,54,55 });

    Tile expectedTile5(3,6,5,9,{ 36,37,38,39,
                                 46,47,48,49,
                                 56,57,58,59 });

    Tile expectedTile6(6,0,9,2,{ 60,61,62,
                                 70,71,72,
                                 80,81,82,
                                 90,91,92 });

    Tile expectedTile7(6,3,9,5,{ 63,64,65,
                                 73,74,75,
                                 83,84,85,
                                 93,94,95 });

    Tile expectedTile8(6,6,9,9,{ 66,67,68,69,
                                 76,77,78,79,
                                 86,87,88,89,
                                 96,97,98,99 });

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

    /* we have a 18 x 7 array to divide into 4 tiles, so here are the
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

    Tile expectedTile0(0,0,2,8,{ 0, 1, 2, 3, 4, 5, 6, 7, 8,
                                 18, 19, 20, 21, 22, 23, 24, 25, 26,
                                 36, 37, 38, 39, 40, 41, 42, 43, 44 });

    Tile expectedTile1(0,9,2,17,{ 9, 10, 11, 12, 13, 14, 15, 16, 17,
                                  27, 28, 29, 30, 31, 32, 33, 34, 35,
                                  45, 46, 47, 48, 49, 50, 51, 52, 53 });

    Tile expectedTile2(3,0,6,8,{ 54, 55, 56, 57, 58, 59, 60, 61, 62,
                                 72, 73, 74, 75, 76, 77, 78, 79, 80,
                                 90, 91, 92, 93, 94, 95, 96, 97, 98,
                                 108, 109, 110, 111, 112, 113, 114, 115, 116 });

    Tile expectedTile3(3,9,6,17,{ 63, 64, 65, 66, 67, 68, 69, 70, 71,
                                  81, 82, 83, 84, 85, 86, 87, 88, 89,
                                  99, 100, 101, 102, 103, 104, 105, 106, 107,
                                  117, 118, 119, 120, 121, 122, 123, 124, 125 });

    ASSERT_EQ(4, tiles.size());
    ASSERT_EQ(expectedTile0, tiles.at(0));
    ASSERT_EQ(expectedTile1, tiles.at(1));
    ASSERT_EQ(expectedTile2, tiles.at(2));
    ASSERT_EQ(expectedTile3, tiles.at(3));
}

TEST(BidimiensionalArrayPartitioner, test18x7in9) {

    /* we have a 18 x 7 array to divide into 9 tiles, so here are the
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

    Tile expectedTile0(0,0,1,5,{ 0, 1, 2, 3, 4, 5,
                                 18, 19, 20, 21, 22, 23 });

    Tile expectedTile1(0,6,1,11,{ 6, 7, 8, 9, 10, 11,
                                  24, 25, 26, 27, 28, 29 });

    Tile expectedTile2(0,12,1,17,{ 12, 13, 14, 15, 16, 17,
                                   30, 31, 32, 33, 34, 35 });

    Tile expectedTile3(2,0,3,5,{ 36, 37, 38, 39, 40, 41,
                                 54, 55, 56, 57, 58, 59 });

    Tile expectedTile4(2,6,3,11,{ 42, 43, 44, 45, 46, 47,
                                  60, 61, 62, 63, 64, 65 });

    Tile expectedTile5(2,12,3,17,{ 48, 49, 50, 51, 52, 53,
                                   66, 67, 68, 69, 70, 71 });

    Tile expectedTile6(4,0,6,5,{ 72, 73, 74, 75, 76, 77,
                                 90, 91, 92, 93, 94, 95,
                                 108, 109, 110, 111, 112, 113 });

    Tile expectedTile7(4,6,6,11,{ 78, 79, 80, 81, 82, 83,
                                  96, 97, 98, 99, 100, 101,
                                  114, 115, 116, 117, 118, 119 });

    Tile expectedTile8(4,12,6,17,{ 84, 85, 86, 87, 88, 89,
                                   102, 103, 104, 105, 106, 107,
                                   120, 121, 122, 123, 124, 125 });

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

TEST(BidimiensionalArrayPartitioner, test18x7in6) {

    /* we have a 18 x 7 array to divide into 6 tiles, so here are the
     * positions and the expected division:
     *
     * 00 01 02 03 04 05 - 06 07 08 09 10 11 - 12 13 14 15 16 17
     * 18 19 20 21 22 23 - 24 25 26 27 28 29 - 30 31 32 33 34 35
     * 36 37 38 39 40 41 - 42 43 44 45 46 47 - 48 49 50 51 52 53
     * -- -- -- -- -- -- -- -- -- - -- -- -- -- -- -- -- -- --
     * 54 55 56 57 58 59 - 60 61 62 63 64 65 - 66 67 68 69 70 71
     * 72 73 74 75 76 77 - 78 79 80 81 82 83 - 84 85 86 87 88 89
     * 90 91 92 93 94 95 - 96 97 98 99 100 101 - 102 103 104 105 106 107
     * 108 109 110 111 112 113 - 114 115 116 117 118 119 - 120 121 122 123 124 125
     */

    vector<Tile> tiles = divideSquareIntoTiles(18,7,6);

    Tile expectedTile0(0,0,2,5,{ 0, 1, 2, 3, 4, 5,
                                 18, 19, 20, 21, 22, 23,
                                 36, 37, 38, 39, 40, 41 });

    Tile expectedTile1(0,6,2,11,{ 6, 7, 8, 9, 10, 11,
                                  24, 25, 26, 27, 28, 29,
                                  42, 43, 44, 45, 46, 47 });

    Tile expectedTile2(0,12,2,17,{ 12, 13, 14, 15, 16, 17,
                                   30, 31, 32, 33, 34, 35,
                                   48, 49, 50, 51, 52, 53 });

    Tile expectedTile3(3,0,6,5,{ 54, 55, 56, 57, 58, 59,
                                 72, 73, 74, 75, 76, 77,
                                 90, 91, 92, 93, 94, 95,
                                 108, 109, 110, 111, 112, 113 });

    Tile expectedTile4(3,6,6,11,{ 60, 61, 62, 63, 64, 65,
                                  78, 79, 80, 81, 82, 83,
                                  96, 97, 98, 99, 100, 101,
                                  114, 115, 116, 117, 118, 119 });

    Tile expectedTile5(3,12,6,17,{ 66, 67, 68, 69, 70, 71,
                                   84, 85, 86, 87, 88, 89,
                                   102, 103, 104, 105, 106, 107,
                                   120, 121, 122, 123, 124, 125 });

    ASSERT_EQ(6, tiles.size());
    ASSERT_EQ(expectedTile0, tiles.at(0));
    ASSERT_EQ(expectedTile1, tiles.at(1));
    ASSERT_EQ(expectedTile2, tiles.at(2));
    ASSERT_EQ(expectedTile3, tiles.at(3));
    ASSERT_EQ(expectedTile4, tiles.at(4));
    ASSERT_EQ(expectedTile5, tiles.at(5));
}

TEST(BidimiensionalArrayPartitioner, test7x18in6) {

    /* we have a 7 x 18 array to divide into 6 tiles, so here are the
     * positions and the expected division:
     *
     * 00 01 02 - 03 04 05 06
     * 07 08 09 - 10 11 12 13
     * 14 15 16 - 17 18 19 20
     * 21 22 23 - 24 25 26 27
     * 28 29 30 - 31 32 33 34
     * 35 36 37 - 38 39 40 41
     * -- -- -- - -- -- -- --
     * 42 43 44 - 45 46 47 48
     * 49 50 51 - 52 53 54 55
     * 56 57 58 - 59 60 61 62
     * 63 64 65 - 66 67 68 69
     * 70 71 72 - 73 74 75 76
     * 77 78 79 - 80 81 82 83
     * -- -- -- - -- -- -- --
     * 84 85 86 - 87 88 89 90
     * 91 92 93 - 94 95 96 97
     * 98 99 100 - 101 102 103 104
     * 105 106 107 - 108 109 110 111
     * 112 113 114 - 115 116 117 118
     * 119 120 121 - 122 123 124 125
     */

    vector<Tile> tiles = divideSquareIntoTiles(7,18,6);

    Tile expectedTile0(0,0,5,2,{ 0, 1, 2,
                                 7, 8, 9,
                                 14, 15, 16,
                                 21, 22, 23,
                                 28, 29, 30,
                                 35, 36, 37 });

    Tile expectedTile1(0,3,5,6,{ 3, 4, 5, 6,
                                 10, 11, 12, 13,
                                 17, 18, 19, 20,
                                 24, 25, 26, 27,
                                 31, 32, 33, 34,
                                 38, 39, 40, 41 });

    Tile expectedTile2(6,0,11,2,{ 42, 43, 44,
                                  49, 50, 51,
                                  56, 57, 58,
                                  63, 64, 65,
                                  70, 71, 72,
                                  77, 78, 79 });

    Tile expectedTile3(6,3,11,6,{ 45, 46, 47, 48,
                                  52, 53, 54, 55,
                                  59, 60, 61, 62,
                                  66, 67, 68, 69,
                                  73, 74, 75, 76,
                                  80, 81, 82, 83 });

    Tile expectedTile4(12,0,17,2,{ 84, 85, 86,
                                   91, 92, 93,
                                   98, 99, 100,
                                   105, 106, 107,
                                   112, 113, 114,
                                   119, 120, 121 });

    Tile expectedTile5(12,3,17,6,{ 87, 88, 89, 90,
                                   94, 95, 96, 97,
                                   101, 102, 103, 104,
                                   108, 109, 110, 111,
                                   115, 116, 117, 118,
                                   122, 123, 124, 125 });

    ASSERT_EQ(6, tiles.size());
    ASSERT_EQ(expectedTile0, tiles.at(0));
    ASSERT_EQ(expectedTile1, tiles.at(1));
    ASSERT_EQ(expectedTile2, tiles.at(2));
    ASSERT_EQ(expectedTile3, tiles.at(3));
    ASSERT_EQ(expectedTile4, tiles.at(4));
    ASSERT_EQ(expectedTile5, tiles.at(5));
}

TEST(BidimiensionalArrayPartitioner, test13x7in8) {

    /* we have a 13 x 7 array to divide into 8 tiles, so here are the
     * positions and the expected division:
     *
     * 00 01 02 - 03 04 05 - 06 07 08 - 09 10 11 12
     * 13 14 15 - 16 17 18 - 19 20 21 - 22 23 24 25
     * 26 27 28 - 29 30 31 - 32 33 34 - 35 36 37 38
     * -- -- -- - -- -- -- - -- -- -- - -- -- -- --
     * 39 40 41 - 42 43 44 - 45 46 47 - 48 49 50 51
     * 52 53 54 - 55 56 57 - 58 59 60 - 61 62 63 64
     * 65 66 67 - 68 69 70 - 71 72 73 - 74 75 76 77
     * 78 79 80 - 81 82 83 - 84 85 86 - 87 88 89 90
     */

    vector<Tile> tiles = divideSquareIntoTiles(13,7,8);

    Tile expectedTile0(0,0,2,2,{ 0, 1, 2,
                                 13, 14, 15,
                                 26, 27, 28 });

    Tile expectedTile1(0,3,2,5,{ 3, 4, 5,
                                 16, 17, 18,
                                 29, 30, 31 });

    Tile expectedTile2(0,6,2,8,{ 6, 7, 8,
                                 19, 20, 21,
                                 32, 33, 34 });

    Tile expectedTile3(0,9,2,12,{ 9, 10, 11, 12,
                                  22, 23, 24, 25,
                                  35, 36, 37, 38 });

    Tile expectedTile4(3,0,6,2,{ 39, 40, 41,
                                 52, 53, 54,
                                 65, 66, 67,
                                 78, 79, 80 });

    Tile expectedTile5(3,3,6,5,{ 42, 43, 44,
                                 55, 56, 57,
                                 68, 69, 70,
                                 81, 82, 83 });

    Tile expectedTile6(3,6,6,8,{ 45, 46, 47,
                                 58, 59, 60,
                                 71, 72, 73,
                                 84, 85, 86 });

    Tile expectedTile7(3,9,6,12,{ 48, 49, 50, 51,
                                  61, 62, 63, 64,
                                  74, 75, 76, 77,
                                  87, 88, 89, 90});

    ASSERT_EQ(8, tiles.size());
    ASSERT_EQ(expectedTile0, tiles.at(0));
    ASSERT_EQ(expectedTile1, tiles.at(1));
    ASSERT_EQ(expectedTile2, tiles.at(2));
    ASSERT_EQ(expectedTile3, tiles.at(3));
    ASSERT_EQ(expectedTile4, tiles.at(4));
    ASSERT_EQ(expectedTile5, tiles.at(5));
    ASSERT_EQ(expectedTile6, tiles.at(6));
    ASSERT_EQ(expectedTile7, tiles.at(7));
}
