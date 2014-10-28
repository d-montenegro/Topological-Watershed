#ifndef __IMAGEREADER_H_INCLUDED__
#define __IMAGEREADER_H_INCLUDED__

#include "Image.h"
#include <vector>

void readImage(const std::string& imagePath, const std::string& format,
               std::vector<ushort>& pixels, unsigned int& width,
               unsigned int& height);

void buildImage(const std::string& , const std::string& ,
                const std::vector<ushort>& , unsigned int& ,
                unsigned int& );

#endif // __IMAGEREADER_H_INCLUDED__

