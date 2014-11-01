#include <cmath>
#include "BidimiensionalArrayPartitioner.h"

using namespace std;

vector<Tile> divideSquareIntoTiles(unsigned int squareWidth,
                                   unsigned int squareHeight,
                                   unsigned int numberOfTiles)
{
    vector<Tile> tiles(numberOfTiles);

    // number of elements in a row of a partition
    unsigned int partitionWidth = round(squareWidth / sqrt(numberOfTiles));
    // number of elements in a column of a partition
    unsigned int partitionHeight = round(squareHeight / sqrt(numberOfTiles));
    // number of partitions in a row
    unsigned int totalWidthPartitions = round((float)squareWidth / (float)partitionWidth);
    // number of partitions in a column
    unsigned int totalHeightPartitions = round((float)squareHeight / (float)partitionHeight);

    unsigned int currentTile = 0;
    unsigned int counter = 1;
    for(unsigned int h = 0; h < squareHeight; h++)
    {
        for(unsigned int w = 0; w < squareWidth; w++)
        {
            unsigned int position = w + squareWidth * h;
            if(position % squareWidth == 0)
            {
                counter = 1;
                currentTile = min((h / partitionHeight) * totalWidthPartitions,
                                  (totalHeightPartitions - 1) * totalWidthPartitions);
            }
            else if(counter == partitionWidth)
            {
                if((currentTile + 1) % totalWidthPartitions != 0)
                {
                    counter = 1;
                    currentTile++;
                }
            }
            else
            {
                counter++;
            }
            tiles.at(currentTile).insert(position);
        }
    }

    return tiles;
}
