#ifndef __IMAGE_H_INCLUDED__
#define __IMAGE_H_INCLUDED__

#include <vector>

using namespace std;

class Image
{
public:
    /*
     * Constructor.
     */
    Image(const vector<unsigned int>& pixels, const unsigned int width,
          const unsigned int height) : pixels(pixels), width(width),
                                       height(height) {}

    /*
     * Returns the indexes at pixels of the neighbors of the pixel at position
     * 'pixelPosition' at pixels. For now, assuming type-4 neighbour relation.
     */
    vector<unsigned int> getNeighbors(unsigned int pixelPosition,
                                      unsigned int lessThan = 0) const;

    vector<unsigned int> getPixels() const { return pixels; }
    unsigned int getWidth() const { return width; }
    unsigned int getHeight() const { return height; }

private:
    vector<unsigned int> pixels;
    const unsigned int width;
    const unsigned int height;
};

#endif // __IMAGE_H_INCLUDED__
