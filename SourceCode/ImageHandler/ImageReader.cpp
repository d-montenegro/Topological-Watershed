#include <iostream>
#include <cstddef>
#include <jpeglib.h>
#include <ImageReader.h>
#include <stdexcept>
#include <cstring>
#include <png++/image.hpp>

void readPngImage(const std::string& imagePath, std::vector<ushort>& pixels,
                  unsigned int& width, unsigned int& height);

void writePngImage(const std::string& imagePath, const std::vector<ushort>& pixels,
                  unsigned int width, unsigned int height);

void readImage(const std::string& imagePath, IMAGE_FORMAT format,
               std::vector<ushort>& pixels, unsigned int& width,
               unsigned int& height)
{
    if (format == PNG)
    {
        pixels.clear();
        readPngImage(imagePath, pixels, width, height);
    }
    else
    {
        throw std::invalid_argument("Unsupported format: " +format);
    }
}

void writeImage(const std::string& imagePath, IMAGE_FORMAT format,
               const std::vector<ushort>& pixels, unsigned int width,
               unsigned int height)
{
    if (format == PNG)
    {
        writePngImage(imagePath, pixels, width, height);
    }
    else
    {
        throw std::invalid_argument("Unsupported format: " +format);
    }
}

void readPngImage(const std::string& imagePath, std::vector<ushort>& pixels,
                  unsigned int& width,
                  unsigned int& height)
{
    png::image<png::gray_pixel> image(imagePath);
    width = image.get_width();
    height = image.get_height();

    for(unsigned int y = 0; y < height; y++)
    {
        for(unsigned int x = 0; x < width; x++)
        {
            pixels.push_back(image.get_pixel(x,y));
        }
    }
}

void writePngImage(const std::string& imagePath, const std::vector<ushort>& pixels,
                   unsigned int width, unsigned int height)
{
    png::image<png::gray_pixel> image(width,height);

    for(unsigned int y = 0; y < height; y++)
    {
        for(unsigned int x = 0; x < width; x++)
        {
            image.set_pixel(x,y,pixels.at(x + y * width));
        }
    }

    try
    {
        image.write(imagePath);
    }
    catch (runtime_error e)
    {
        throw runtime_error("Could not write on: \"" + imagePath +
                            "\". Reason: " + string(e.what()));
    }
}
