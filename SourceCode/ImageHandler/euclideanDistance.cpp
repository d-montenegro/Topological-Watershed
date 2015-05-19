#include <iostream>
#include <stdexcept>
#include <cstring>
#include <string>
#include <random>
#include <cstdlib>


#include "ImageReader.h"

using namespace std;

void parseImageSize(string imageSize, ushort& width, ushort& height)
{
    size_t position = 0;
    position = imageSize.find('x');
    if (position == string::npos)
    {
        position = imageSize.find('X');
        if (position == string::npos)
        {
            throw invalid_argument("Could not parse size: " +imageSize);
        }
    }

    width = atoi(imageSize.substr(0,position).c_str());
    height = atoi(imageSize.substr(position+1).c_str());
}

void printUsage(string binaryName)
{
    cout << "Usage is " << binaryName <<
            " <image_one> <image_two> " << endl << endl
         << "image_one a PNG imagen" << endl
         << "image_two another PNG image" << endl << endl;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        cout << "Wrong params." << endl;
        printUsage(argv[0]);
        return 0;
    }

    vector<ushort> pixelsImageOne, pixelsImageTwo;
    unsigned int widthOne, heightOne, widthTwo, heightTwo;

    try
    {
        readImage(argv[1], PNG, pixelsImageOne, widthOne, heightOne);
        readImage(argv[2], PNG, pixelsImageTwo, widthTwo, heightTwo);
    }
    catch(std::exception&)
    {
        cout << "Could not read image" << endl;
        printUsage(argv[0]);
        return 0;
    }

    if (widthOne != widthTwo || heightOne != heightTwo)
    {
        cout << "Both images must be of the same resolution" << endl;
        printUsage(argv[0]);
        return 0;
    }

    unsigned int total = 0;

    unsigned int sizes = pixelsImageOne.size();
    for(unsigned int i = 0; i < sizes; i++)
    {
        total += abs(pixelsImageOne.at(i) - pixelsImageTwo.at(i));
    }

    cout << total / (float)sizes << endl;
    return 0;
}
