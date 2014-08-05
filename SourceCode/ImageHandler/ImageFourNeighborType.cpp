#include <limits>
#include "ImageFourNeighborType.h"

using namespace std;

vector<unsigned int> ImageFourNeighborType::getNeighbors(
        unsigned int pixelPosition) const
{
    return getNeighbors(pixelPosition, numeric_limits<ushort>::max());
}

/*
 * Returns the indexes at pixels of the neighbors of the pixel at position
 * 'pixelPosition' which value is lower or equal than its value.
 */
vector<unsigned int> ImageFourNeighborType::getLowerOrEqualNeighbors(
        unsigned int pixelPosition) const
{
    return getNeighbors(pixelPosition, pixels.at(pixelPosition));
}

vector<unsigned int> ImageFourNeighborType::getNeighbors(
        unsigned int pixelPosition, ushort maxValue) const
{
    ushort greyLevel = pixels.at(pixelPosition);
    vector<unsigned int> neighbors;


    if (pixelPosition >= width)
    {
        // I'm not in row 0, then I have a neighbour in the up row
        if (greyLevel <= maxValue)
        {
            neighbors.push_back(pixelPosition - width);
        }
    }

    if (pixelPosition < width * (height - 1))
    {
        // I'm not last row, then I have a neighbour in the previous row
        if (greyLevel <= maxValue)
        {
            neighbors.push_back(pixelPosition + width);
        }
    }

    if (0 != pixelPosition % width)
    {
        // I'm not in the first column, then I have a neighbour on the left
        if (greyLevel <= maxValue)
        {
            neighbors.push_back(pixelPosition - 1);
        }
    }

    if (0 != (pixelPosition + 1) % width)
    {
        // I'm not in the last column, then I have a neighbour on the right
        if (greyLevel <= maxValue)
        {
            neighbors.push_back(pixelPosition + 1);
        }
    }

    return neighbors;
}
