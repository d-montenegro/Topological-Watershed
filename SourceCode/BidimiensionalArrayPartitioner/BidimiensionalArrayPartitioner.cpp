#include <cmath>
#include "BidimiensionalArrayPartitioner.h"

using namespace std;

unsigned int specialRound(float number)
{
    unsigned int numberToInteger = number;
    if(number - numberToInteger <= 0.5)
    {
        return numberToInteger;
    }
    return numberToInteger + 1;
}

void getDivisors(unsigned int n, unsigned int& min, unsigned int& max)
{
    unsigned int squareRoot = sqrt(n);
    for(unsigned int i = squareRoot; i > 0; i++)
    {
        if(n % i == 0)
        {
            min = i;
            max = n / i;
            break;
        }
    }
    return;
}

vector<Tile> divideSquareIntoTiles(unsigned int squareWidth,
                                   unsigned int squareHeight,
                                   unsigned int numberOfTiles)
{
    vector<Tile> tiles(numberOfTiles);
    unsigned int minDivisor = 0, maxDivisor = 0;
    getDivisors(numberOfTiles,minDivisor,maxDivisor);

    unsigned int partitionWidth = 0, // number of elements in a row of a partition
            partitionHeight = 0, // number of elements in a column of a partition
            totalWidthPartitions = 0, // number of partitions in a row
            totalHeightPartitions = 0; // number of partitions in a column

    if(squareWidth >= squareHeight)
    {
        partitionWidth = specialRound((float)squareWidth / (float)maxDivisor);
        partitionHeight = specialRound((float)squareHeight / (float)minDivisor);
        totalWidthPartitions = maxDivisor;
        totalHeightPartitions = minDivisor;
    }
    else
    {
        partitionWidth = specialRound((float)squareWidth / (float)minDivisor);
        partitionHeight = specialRound((float)squareHeight / (float)maxDivisor);
        totalWidthPartitions = minDivisor;
        totalHeightPartitions = maxDivisor;
    }

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
