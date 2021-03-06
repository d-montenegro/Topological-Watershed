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
    cout << "Usage is " << binaryName <<
            " -in <input> -out <output> -r <neighbor_relation> [-t <thread_number>]." << endl  << endl
         << "\tinput \tinput PNG greyscale image." << endl
         << "\toutput \toutput PNG greyscale image." << endl
         << "\tneigbor_relation \t4 to 4-neighbor relation type, or 8 to 8-neighbor relation type." << endl
         << "\tthread_number \t the number of threads to use. 1 by default." << endl << endl
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
        Image* image_orig = 0;
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

        // WCREST
        vector<bool> crest(image->getSize(),false);
        for(unsigned int i = 0; i < image->getSize(); i++)
        {
            bool isMinimum = image->getLowerNeighbors(i).empty() && tree.isLeaf(tree.getComponent(i));
            if (!isMinimum)
            {
                crest.at(i) = true;
            }
        }

        delete image;
        image = 0;

        readImage(sourceImage,PNG,pixels,width,height);
        for(unsigned int i = 0; i < crest.size(); i++)
        {
            if(crest.at(i))
            {
                // white
                pixels.at(i) = 255;
            }
        }

        time(&end);
        cout << "Topological Watershed performed - " <<
                (double)difftime(end,start) << " sec" << endl;

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

