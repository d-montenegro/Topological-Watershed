#ifndef __IMAGE_FOUR_NEIGHBOR_TYPE_H_INCLUDED__
#define __IMAGE_FOUR_NEIGHBOR_TYPE_H_INCLUDED__

#include <vector>

#include "Image.h"

using namespace std;

class ImageFourNeighborType : public Image
{
public:
    ImageFourNeighborType(const vector<ushort>& pixels,
                          const unsigned int width, const unsigned int height)
        : Image(pixels,width,height) {}
    /*
     * Returns the indexes at pixels of the neighbors of the pixel at position
     * 'pixelPosition'.
     */
    set<unsigned int> getNeighbors(unsigned int pixelPosition) const;

    /*
     * Returns the indexes at pixels of the neighbors of the pixel at position
     * 'pixelPosition' which value is lower or equal than its value.
     */
    set<unsigned int> getLowerOrEqualNeighbors(unsigned int pixelPosition) const;

    /*
     * Returns the indexes at pixels of the neighbors of the pixel at position
     * 'pixelPosition' which value is lower than its value.
     */
    set<unsigned int> getLowerNeighbors(unsigned int pixelPosition) const;

private:
    set<unsigned int> getNeighbors(unsigned int pixelPosition,
                                      ushort maxValue) const;

};

#endif // __IMAGE_FOUR_NEIGHBOR_TYPE_H_INCLUDED__
