#ifndef __BIDIMENSIONALARRAYPARTITIONER_H_INCLUDED__
#define __BIDIMENSIONALARRAYPARTITIONER_H_INCLUDED__

#include <set>
#include <vector>

using Tile = std::set<unsigned int>;

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
