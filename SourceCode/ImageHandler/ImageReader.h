#ifndef __IMAGEREADER_H_INCLUDED__
#define __IMAGEREADER_H_INCLUDED__

#include "Image.h"
#include <vector>

void readImage(const std::string& imagePath, const std::string& format,
               std::vector<ushort>& pixels, unsigned int& width,
               unsigned int& height);

void buildImage(const std::string& destinationImagePath, const std::string& format,
                const std::vector<ushort>& pixels, unsigned int& width,
                unsigned int& height);

#endif // __IMAGEREADER_H_INCLUDED__

