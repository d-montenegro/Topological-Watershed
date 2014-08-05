#ifndef __IMAGE_H_INCLUDED__
#define __IMAGE_H_INCLUDED__

#include <vector>

using ushort = unsigned short;

using namespace std;

class Image
{
public:
    /*
     * Constructor.
     */
    Image(const vector<ushort>& pixels, const unsigned int width,
          const unsigned int height, const ushort lowestGreyIntensity,
          const ushort highestGreyIntensity) : pixels(pixels), width(width),
        height(height), lowestGreyIntensity(lowestGreyIntensity),
        highestGreyIntensity(highestGreyIntensity) {}

    virtual ~Image() {}

    /*
     * Returns the indexes at pixels of the neighbors of the pixel at position
     * 'pixelPosition'.
     */
    virtual vector<unsigned int> getNeighbors(unsigned int pixelPosition) const = 0;

    /*
     * Returns the indexes at pixels of the neighbors of the pixel at position
     * 'pixelPosition' which value is lower or equal than its value.
     */
    virtual vector<unsigned int> getLowerOrEqualNeighbors(unsigned int pixelPosition) const = 0;

    vector<ushort> getPixels() const { return pixels; }
    unsigned int getWidth() const { return width; }
    unsigned int getHeight() const { return height; }
    ushort getLowestGreyIntensity() const { return lowestGreyIntensity; }
    ushort getHighestGreyIntensity() const { return highestGreyIntensity; }

protected:
    vector<ushort> pixels;
    const unsigned int width;
    const unsigned int height;
    const ushort lowestGreyIntensity;
    const ushort highestGreyIntensity;
};

#endif // __IMAGE_H_INCLUDED__
