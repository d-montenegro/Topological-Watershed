#ifndef __IMAGE_H_INCLUDED__
#define __IMAGE_H_INCLUDED__

#include <iostream>
#include <vector>
#include <set>
#include <limits>
#include <stdexcept>

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
        height(height), lowestGreyIntensity(std::numeric_limits<ushort>::max()),
        highestGreyIntensity(std::numeric_limits<ushort>::min())
    {

        if (0 == width)
        {
            throw invalid_argument("width = 0");
        }
        if (0 == height)
        {
            throw invalid_argument("height = 0");
        }
        if (pixels.size() != width * height)
        {
            throw invalid_argument("Total pixels does not match width * height.");
        }

        for (auto& greyValue : pixels)
        {
            if (greyValue < lowestGreyIntensity)
            {
                lowestGreyIntensity = greyValue;
            }
            if (greyValue > highestGreyIntensity)
            {
                highestGreyIntensity = greyValue;
            }
        }
    }

    virtual ~Image() {}

    /*
     * Returns the indexes at pixels of the neighbors of the pixel at position
     * 'pixelPosition'.
     */
    virtual set<unsigned int> getNeighbors(unsigned int pixelPosition) const = 0;

    /*
     * Returns the indexes at pixels of the neighbors of the pixel at position
     * 'pixelPosition' which value is lower or equal than its value.
     */
    virtual set<unsigned int> getLowerOrEqualNeighbors(unsigned int pixelPosition) const = 0;

    /*
     * Returns the indexes at pixels of the neighbors of the pixel at position
     * 'pixelPosition' which value is lower than its value.
     */
    virtual set<unsigned int> getLowerNeighbors(unsigned int pixelPosition) const = 0;

    void printMe()
    {
        for(unsigned int index = 0; index < pixels.size(); index++)
        {
            if(index != 0 && index % width == 0)
                cout << endl;
            if(highestGreyIntensity >= 10 && pixels.at(index) < 10)
                cout << 0;
            cout << pixels.at(index) << " ";
        }
    }

    vector<ushort> getPixels() const { return pixels; }
    unsigned int getWidth() const { return width; }
    unsigned int getHeight() const { return height; }
    ushort getLowestGreyIntensity() const { return lowestGreyIntensity; }
    ushort getHighestGreyIntensity() const { return highestGreyIntensity; }
    void setPixelValue(unsigned int pixelPosition, ushort newGreyValue) { pixels.at(pixelPosition) = newGreyValue; }

protected:
    vector<ushort> pixels;
    const unsigned int width;
    const unsigned int height;
    ushort lowestGreyIntensity;
    ushort highestGreyIntensity;
};

#endif // __IMAGE_H_INCLUDED__
