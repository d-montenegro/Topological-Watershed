#ifndef __BIDIMENSIONALARRAYPARTITIONER_H_INCLUDED__
#define __BIDIMENSIONALARRAYPARTITIONER_H_INCLUDED__

#include <utility>
#include <vector>
#include <set>

/*
 * Tile contains the coordinates for a square
 */
struct Tile {
    std::pair<unsigned int,unsigned int> begin;
    std::pair<unsigned int,unsigned int> end;
    std::set<unsigned int> points;

    Tile(unsigned int x = 0, unsigned int y = 0, unsigned int k = 0,
         unsigned int z = 0, std::set<unsigned int> points = { }) : begin(x,y),
         end(k,z), points(points) { }

    bool operator== (const Tile& t) const
    {
        return begin == t.begin && end == t.end && points == t.points;
    }
};

/*
 * Returns optimal partition of a width x heigh square in n, where each partition
 * is partitionWidth x partitionHeight, except maybe the corner partitions.
 * Optimal means distributing as equal as possible, and also being partitionWidth
 * and partitionHeight as close as possible each other.
 */
std::vector<Tile> divideSquareIntoTiles(unsigned int squareWidth,
                                        unsigned int squareHeight,
                                        unsigned int numberOfTiles);

#endif // __BIDIMENSIONALARRAYPARTITIONER_H_INCLUDED__
