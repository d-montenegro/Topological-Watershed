#include <iostream>
#include <stdexcept>
#include <cstring>
#include <string>
#include <random>


#include "ImageReader.h"

using namespace std;

enum RELATION_TYPE
{
    TYPE_4 = 0,
    TYPE_8
};

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
            " -s <image_size> -f <format> -out <dest_image_path> " << endl << endl
         << "image_size is the size of the random image in format <width>x<height>" << endl
         << "format is the format of the source image, may be JPG, JPEG or PNG" << endl
         << "dest_image_path is the path where the image will be wrote" << endl << endl;
}

int main(int argc, char* argv[])
{
    if (argc != 7)
    {
        cout << "Wrong params." << endl;
        printUsage(argv[0]);
        return 0;
    }

    string destinationImage;
    IMAGE_FORMAT format;
    ushort width = 0, height = 0;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i],"-s") == 0)
        {
            try
            {
                parseImageSize(argv[i+1], width, height);
            }
            catch(...)
            {
                cout << "Wrong size: " << argv[i+1] << endl;
                printUsage(argv[0]);
                return 0;
            }
        }
        else if (strcmp(argv[i],"-f") == 0)
        {
            if(strcmp(argv[i+1],"JPG") == 0)
            {
                format = JPG;
            }
            else if(strcmp(argv[i+1],"JPEG") == 0)
            {
                format = JPEG;
            }
            else if(strcmp(argv[i+1],"PNG") == 0)
            {
                format = PNG;
            }
            else
            {
                cout << "Uknown image format: " << argv[i+1] << endl;
                printUsage(argv[0]);
                return 0;
            }
        }
        else if (strcmp(argv[i],"-out") == 0)
        {
            destinationImage = argv[i+1];
        }
        else
        {
            cout << "Unknown param: " << argv[i] << "." << endl;
            printUsage(argv[0]);
            return 0;
        }
        i++;
    }

    default_random_engine generator;
    uniform_int_distribution<ushort> distribution(0,255);

    unsigned int vectorSize = width*height;
    vector<ushort> pixels;
    for(unsigned int i = 0; i < vectorSize; i++)
    {
        pixels.push_back(distribution(generator));
    }

    writeImage(destinationImage,format,pixels,width,height);

    return 0;
}
