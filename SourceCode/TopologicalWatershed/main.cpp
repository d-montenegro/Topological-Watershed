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
    TYPE_4 = 0,
    TYPE_8
};

void printUsage(string binaryName)
{
    cout << "Usage is " << binaryName <<
            " -in <source_image_path> -out <dest_image_path> -r <neighbor_relation>\
 [-t <thread_number>]" << endl  << endl
         << "source_image_path is the path to a greyscale. Must be PNG format" << endl
         << "dest_image_path is the path where the processed image will be wrote" << endl
         << "neigbor_relation may be 4 for 4-neighbor relation type, or 8 "
         << "for 8-neighbor relation type" << endl
         << "thread_number specifies the number of threads to use. Will be 1 if not specified" << endl << endl;
}

int main(int argc, char* argv[])
{
    if (argc != 7 && argc != 9)
    {
        cout << "Wrong params." << endl;
        printUsage(argv[0]);
        return 0;
    }

    string sourceImage, destinationImage;
    RELATION_TYPE relationType;

    unsigned short threads = 1;
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
                return 0;
            }
        }
        else if(strcmp(argv[i],"-t") == 0)
        {
            int threadsToUse = atoi(argv[i+1]);
            if(threadsToUse < 1)
            {
                cout << "Threads number must be equal or higher than 1" << endl;
                printUsage(argv[0]);
                return 0;
            }
            threads = threadsToUse;
        }
        else
        {
            cout << "Unknown param: " << argv[i] << "." << endl;
            printUsage(argv[0]);
            return 0;
        }
        i++;
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

        time_t start,end;
        time (&start);

        ComponentTree tree(*image);
        time(&end);
        cout << "Component Tree done - " << (double)difftime(end,start) << " sec" << endl;

        if(threads == 1)
        {
            doLinearTopologicalWatershed(*image,tree);
        }
        else
        {
            doParallelTopologicalWatershed(*image, tree, threads);
        }
        time(&end);
        cout << "Topological Watershed performed - " <<
                (double)difftime(end,start) << " sec" << endl;

        writeImage(destinationImage,PNG,image->getPixels(),
                   image->getWidth(),image->getHeight());

        delete image;
        image = 0;
    }
    catch (const bad_alloc&)
    {
        cerr << "Not enough memory to continue."
             << endl;
    }
    catch (const exception& e)
    {
        cerr << "An unexpected error occurs. Detail: " << e.what() << endl;
    }

    return 0;
}

