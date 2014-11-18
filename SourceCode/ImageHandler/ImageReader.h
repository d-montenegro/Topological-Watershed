#ifndef __IMAGEREADER_H_INCLUDED__
#define __IMAGEREADER_H_INCLUDED__

#include "Image.h"
#include <vector>

enum IMAGE_FORMAT
{
    JPG = 0,
    JPEG,
    PNG
};

void readImage(const std::string& imagePath, IMAGE_FORMAT format,
               std::vector<ushort>& pixels, unsigned int& width,
               unsigned int& height);

void writeImage(const std::string& imagePath, IMAGE_FORMAT format,
                const std::vector<ushort>& pixels, unsigned int width,
                unsigned int height);

#endif // __IMAGEREADER_H_INCLUDED__

