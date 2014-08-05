#ifndef __IMAGE_FOUR_NEIGHBOR_TYPE_H_INCLUDED__
#define __IMAGE_FOUR_NEIGHBOR_TYPE_H_INCLUDED__

#include <vector>

#include "Image.h"

using namespace std;

class ImageFourNeighborType : public Image
{
public:
    ImageFourNeighborType(const vector<unsigned int>& pixels,
                          const unsigned int width, const unsigned int height)
        : Image(pixels,width,height) {}
    /*
     * Returns the indexes at pixels of the neighbors of the pixel at position
     * 'pixelPosition'.
     */
    vector<unsigned int> getNeighbors(unsigned int pixelPosition) const;

    /*
     * Returns the indexes at pixels of the neighbors of the pixel at position
     * 'pixelPosition' which value is lower or equal than its value.
     */
    vector<unsigned int> getLowerOrEqualNeighbors(unsigned int pixelPosition) const;

private:
    vector<unsigned int> getNeighbors(unsigned int pixelPosition,
                                      unsigned int maxValue) const;

};

#endif // __IMAGE_FOUR_NEIGHBOR_TYPE_H_INCLUDED__
