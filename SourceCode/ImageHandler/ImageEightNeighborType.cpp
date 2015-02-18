#include <limits>
#include "ImageEightNeighborType.h"

using namespace std;

set<unsigned int> ImageEightNeighborType::getNeighbors(
        unsigned int pixelPosition) const
{
    return getNeighbors(pixelPosition, numeric_limits<ushort>::max());
}

/*
 * Returns the indexes at pixels of the neighbors of the pixel at position
 * 'pixelPosition' which value is lower or equal than its value.
 */
set<unsigned int> ImageEightNeighborType::getLowerOrEqualNeighbors(
        unsigned int pixelPosition) const
{
    return getNeighbors(pixelPosition, pixels.at(pixelPosition));
}

/*
 * Returns the indexes at pixels of the neighbors of the pixel at position
 * 'pixelPosition' which value is lower or equal than its value.
 */
set<unsigned int> ImageEightNeighborType::getLowerNeighbors(
        unsigned int pixelPosition) const
{
    if (lowestGreyIntensity == pixels.at(pixelPosition))
    {
        return set<unsigned int>();
    }
    return getNeighbors(pixelPosition, pixels.at(pixelPosition) - 1);
}

set<unsigned int> ImageEightNeighborType::getNeighbors(
        unsigned int pixelPosition, ushort maxValue) const
{
    set<unsigned int> neighbors;
    unsigned int raw = 0, col = 0;

    getCoordinates(pixelPosition,raw,col);

    if(raw > 0)
    {
        // It's not raw 0 so there are neighbors in the previous raw
        if (at(raw - 1, col) <= maxValue)
        {
            neighbors.insert(getAbsolute(raw-1,col));
        }

        if(col > 0)
        {
            // It's not col 0, so there's a neighbor in the previous col
            if (at(raw - 1, col - 1) <= maxValue)
            {
                neighbors.insert(getAbsolute(raw-1,col-1));
            }
        }
        if(col + 1 < width)
        {
            // It's not last col, so there's a neighbor in the next col
            if (at(raw - 1, col + 1) <= maxValue)
            {
                neighbors.insert(getAbsolute(raw-1,col+1));
            }
        }
    }

    if (col > 0)
    {
        // It's not in the first column, then I have a neighbour on the left
        if (at(raw,col-1) <= maxValue)
        {
            neighbors.insert(getAbsolute(raw,col-1));
        }
    }

    if (col + 1 < width)
    {
        // It's not the last column, then I have a neighbour on the right
        if (at(raw,col+1) <= maxValue)
        {
            neighbors.insert(getAbsolute(raw,col+1));
        }
    }

    if (raw + 1 < height)
    {
        // It's not the last row, then I have a neighbour in the next row
        if (at(raw + 1, col) <= maxValue)
        {
            neighbors.insert(getAbsolute(raw+1,col));
        }

        if(col > 0)
        {
            // It's not col 0, so there's a neighbor in the previous col
            if (at(raw + 1, col - 1) <= maxValue)
            {
                neighbors.insert(getAbsolute(raw+1,col-1));
            }
        }
        if(col + 1 < width)
        {
            // It's not last col, so there's a neighbor in the next col
            if (at(raw + 1, col + 1) <= maxValue)
            {
                neighbors.insert(getAbsolute(raw+1,col+1));
            }
        }
    }

    return neighbors;
}
