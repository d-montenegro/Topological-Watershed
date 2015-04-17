#include <iostream>
#include <stdexcept>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <exception>

#include "TopologicalWatershed.h"
#include "ImageReader.h"
#include "ImageFourNeighborType.h"
#include "ImageEightNeighborType.h"

using namespace std;

enum RELATION_TYPE
{
    UNKNOWN = 0,
    TYPE_4,
    TYPE_8
};

void printUsage(string binaryName)
{
    cout << "Usage is " << binaryName << " -in <input> -out <output> " << endl  << endl
         << "\tinput \tinput PNG image." << endl
         << "\toutput \toutput PNG greyscale image." << endl
         << "Example: " << binaryName << " -in source.png -out destination.png" << endl << endl;
}

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        printUsage(argv[0]);
        return 0;
    }

    if (argc != 5)
    {
        cout << "Wrong params." << endl;
        printUsage(argv[0]);
        return 1;
    }

    string sourceImage, destinationImage;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i],"-in") == 0)
        {
            sourceImage = argv[i+1];
        }
        else if (strcmp(argv[i],"-out") == 0)
        {
            destinationImage = argv[i+1];
        }
        else
        {
            cout << "Unknown param: " << argv[i] << "." << endl;
            printUsage(argv[0]);
            return 1;
        }
        i++;
    }

    if(sourceImage.empty())
    {
        cout << "missing source image" << endl;
        printUsage(argv[0]);
        return 1;
    }

    if(destinationImage.empty())
    {
        cout << "missing destination image" << endl;
        printUsage(argv[0]);
        return 1;
    }

    try
    {
        vector<ushort> pixels;
        unsigned int width = 0;
        unsigned int height = 0;
        readImage(sourceImage,PNG,pixels,width,height);
        writeImage(destinationImage,PNG,pixels,width,height);
    }
    catch (const bad_alloc&)
    {
        cout << "Not enough memory to continue." << endl;
        return 1;
    }
    catch (const exception& e)
    {
        cout << "Unexpected error occurs. Detail: " << e.what() << endl;
        return 1;
    }

    return 0;
}

