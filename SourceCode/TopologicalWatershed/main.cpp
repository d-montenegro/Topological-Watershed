#include <iostream>
#include <stdexcept>
#include <cstring>
#include <cstdlib>
#include <sys/time.h>
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
    cout << "Usage is " << binaryName <<
            " -in <input> -out <output> -r <neighbor_relation> [-t <thread_number>]." << endl  << endl
         << "\tinput \tinput PNG greyscale image." << endl
         << "\toutput \toutput PNG greyscale image." << endl
         << "\tneigbor_relation \t4 to 4-neighbor relation type, or 8 to 8-neighbor relation type." << endl
         << "\tthread_number \t the number of threads to use. If not specified, sequential algorithm will run." << endl << endl
         << "Example: " << binaryName << " -in source.png -out destination.png -r 4 -t 8" << endl << endl;
}

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        printUsage(argv[0]);
        return 0;
    }

    if (argc != 7 && argc != 9)
    {
        cout << "Wrong params." << endl;
        printUsage(argv[0]);
        return 1;
    }

    string sourceImage, destinationImage;
    RELATION_TYPE relationType;

    unsigned short threads = 0;
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
        else if (strcmp(argv[i],"-r") == 0)
        {
            if(strcmp(argv[i+1],"4") == 0)
            {
                relationType = TYPE_4;
            }
            else if(strcmp(argv[i+1],"8") == 0)
            {
                relationType = TYPE_8;
            }
            else
            {
                cout << "Uknown neighbor relation type: " << argv[i+1] << endl;
                printUsage(argv[0]);
                return 1;
            }
        }
        else if(strcmp(argv[i],"-t") == 0)
        {
            int threadsToUse = atoi(argv[i+1]);
            if(threadsToUse < 1)
            {
                cout << "Threads number must be equal or higher than 1" << endl;
                printUsage(argv[0]);
                return 1;
            }
            threads = threadsToUse;
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

    if(relationType == UNKNOWN)
    {
        cout << "missing neighbor relation type" << endl;
        printUsage(argv[0]);
        return 1;
    }

    vector<ushort> pixels;
    unsigned int width = 0;
    unsigned int height = 0;

    try
    {
        readImage(sourceImage,PNG,pixels,width,height);

        Image* image = 0;
        if(relationType == TYPE_4)
        {
            image = new ImageFourNeighborType(pixels,width,height);
        }
        else if(relationType == TYPE_8)
        {
            image = new ImageEightNeighborType(pixels,width,height);
        }
        else
        {
            throw invalid_argument("Invalid neighbor relation type");
        }

        ComponentTree tree(*image);

        static struct timeval tm1;
        gettimeofday(&tm1, NULL);
        if(threads == 0)
        {
            doLinearTopologicalWatershed(*image,tree);
        }
        else
        {
            doParallelTopologicalWatershed(*image, tree, threads);
        }

        struct timeval tm2;
        gettimeofday(&tm2, NULL);
        double t = (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec) / 1000000.;
        cout << "Topological Watershed performed - " <<
                t   << " sec" << endl;

        writeImage(destinationImage,PNG,image->getPixels(),
                   image->getWidth(),image->getHeight());

        delete image;
        image = 0;
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

