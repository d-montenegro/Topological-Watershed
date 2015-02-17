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
    for(unsigned int i = squareRoot; i > 0; i--)
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

    unsigned int i = 0;
    unsigned int th = 0;
    for(unsigned int h = 0; h < squareHeight;)
    {
        unsigned int tw = 0;
        th++;
        for(unsigned int w = 0; w < squareWidth;)
        {
            tw++;
            tiles.at(i).begin.first = h;
            tiles.at(i).begin.second = w;
            tiles.at(i).end.first = min(h + partitionHeight - 1,squareHeight - 1);
            tiles.at(i).end.second = min(w + partitionWidth - 1,squareWidth - 1);

            if(tw == totalWidthPartitions)
            {
                tiles.at(i).end.second = squareWidth - 1;
                w = squareWidth;
            }

            if(th == totalHeightPartitions)
            {
                tiles.at(i).end.first = squareHeight - 1;
            }

            w += partitionWidth;
            i++;
        }
        h += partitionHeight;
        if(th == totalHeightPartitions)
        {
            h = squareHeight;
        }
    }

    // calculate points
    set<unsigned int> points;

    for(auto& t : tiles)
    {
        for(unsigned int raw = t.begin.first; raw <= t.end.first; raw++)
        {
            for(unsigned int col = t.begin.second; col <= t.end.second; col++)
            {
                t.points.insert(raw * squareWidth + col);
            }
        }
    }

    return tiles;
}
