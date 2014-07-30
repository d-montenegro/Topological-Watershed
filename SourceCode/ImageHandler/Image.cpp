#include "Image.h"

using namespace std;

// For now assuming 4-neighbour relation
vector<unsigned int> Image::getNeighbors(unsigned int pixelPosition,
                                              unsigned int lessThan) const
{
    unsigned int greyLevel = pixels.at(pixelPosition);
    vector<unsigned int> neighbors;


    if (pixelPosition >= width)
    {
        // I'm not in row 0, then I have a neighbour in the up row
        if (0 == lessThan || greyLevel < lessThan)
        {
            neighbors.push_back(pixelPosition - width);
        }
    }

    if (pixelPosition < width * (height - 1))
    {
        // I'm not last row, then I have a neighbour in the previous row
        if (0 == lessThan || greyLevel < lessThan)
        {
            neighbors.push_back(pixelPosition + width);
        }
    }

    if (0 != pixelPosition % width)
    {
        // I'm not in the first column, then I have a neighbour on the left
        if (0 == lessThan || greyLevel < lessThan)
        {
            neighbors.push_back(pixelPosition - 1);
        }
    }

    if (0 != (pixelPosition + 1) % width)
    {
        // I'm not in the last column, then I have a neighbour on the right
        if (0 == lessThan || greyLevel < lessThan)
        {
            neighbors.push_back(pixelPosition + 1);
        }
    }

    return neighbors;
}
