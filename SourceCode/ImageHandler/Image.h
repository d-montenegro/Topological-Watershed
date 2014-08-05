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
          const unsigned int height) : pixels(pixels), width(width),
                                       height(height) {}

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

protected:
    vector<ushort> pixels;
    const unsigned int width;
    const unsigned int height;
};

#endif // __IMAGE_H_INCLUDED__
