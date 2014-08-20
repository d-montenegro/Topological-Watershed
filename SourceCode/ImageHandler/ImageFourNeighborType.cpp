#include <limits>
#include "ImageFourNeighborType.h"

using namespace std;

set<unsigned int> ImageFourNeighborType::getNeighbors(
        unsigned int pixelPosition) const
{
    return getNeighbors(pixelPosition, numeric_limits<ushort>::max());
}

/*
 * Returns the indexes at pixels of the neighbors of the pixel at position
 * 'pixelPosition' which value is lower or equal than its value.
 */
set<unsigned int> ImageFourNeighborType::getLowerOrEqualNeighbors(
        unsigned int pixelPosition) const
{
    return getNeighbors(pixelPosition, pixels.at(pixelPosition));
}

/*
 * Returns the indexes at pixels of the neighbors of the pixel at position
 * 'pixelPosition' which value is lower or equal than its value.
 */
set<unsigned int> ImageFourNeighborType::getLowerNeighbors(
        unsigned int pixelPosition) const
{
    if (0 == pixels.at(pixelPosition))
    {
        return set<unsigned int>();
    }
    return getNeighbors(pixelPosition, pixels.at(pixelPosition) - 1);
}

set<unsigned int> ImageFourNeighborType::getNeighbors(
        unsigned int pixelPosition, ushort maxValue) const
{
    set<unsigned int> neighbors;
    if (pixelPosition >= width)
    {
        // I'm not in row 0, then I have a neighbour in the up row
        if (pixels.at(pixelPosition - width) <= maxValue)
        {
            neighbors.insert(pixelPosition - width);
        }
    }

    if (pixelPosition < width * (height - 1))
    {
        // I'm not last row, then I have a neighbour in the previous row
        if (pixels.at(pixelPosition + width) <= maxValue)
        {
            neighbors.insert(pixelPosition + width);
        }
    }

    if (0 != pixelPosition % width)
    {
        // I'm not in the first column, then I have a neighbour on the left
        if (pixels.at(pixelPosition - 1) <= maxValue)
        {
            neighbors.insert(pixelPosition - 1);
        }
    }

    if (0 != (pixelPosition + 1) % width)
    {
        // I'm not in the last column, then I have a neighbour on the right
        if (pixels.at(pixelPosition + 1) <= maxValue)
        {
            neighbors.insert(pixelPosition + 1);
        }
    }

    return neighbors;
}
