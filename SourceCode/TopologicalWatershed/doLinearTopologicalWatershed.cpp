#include <iostream>
#include <stdexcept>
#include <cstring>

#include "TopologicalWatershed.h"
#include "ImageReader.h"
#include "ImageFourNeighborType.h"

using namespace std;

enum RELATION_TYPE
{
    TYPE_4 = 0,
    TYPE_8
};

void printUsage(string binaryName)
{
    cout << "Usage is " << binaryName <<
            " -in <source_image_path> -f <source_image_format> -out <dest_image_path>\
            -r <neighbor_relation>" << endl  << endl
         << "source_image_path is the path to a greyscale" << endl
         << "source_image_format is the format of the source image, may be JPG, JPEG or PNG" << endl
         << "dest_image_path is the path where the processed image will be wrote" << endl
         << "neigbor_relation may be 4 for 4-neighbor relation type, or 8 "
         << "for 8-neighbor relation type" << endl << endl;
}

int main(int argc, char* argv[])
{
    if (argc != 9)
    {
        cout << "Wrong params." << endl;
        printUsage(argv[0]);
        return 0;
    }

    string sourceImage, destinationImage;
    RELATION_TYPE relationType;
    IMAGE_FORMAT format;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i],"-in") == 0)
        {
            sourceImage = argv[i+1];
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
        else if (strcmp(argv[i],"-r"))
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

    const clock_t begin_time = clock();
    readImage(sourceImage,format,pixels,width,height);

    cout << "Image readed - " <<
            float( clock () - begin_time ) /  CLOCKS_PER_SEC << endl;

    Image* image = 0;
    if(relationType == TYPE_4)
    {
        image = new ImageFourNeighborType(pixels,width,height);
    }
    else
    {
        throw invalid_argument("8_TYPE neighbor relation unimplemented yet.");
    }

    cout << "Image instance created - " <<
            float( clock () - begin_time ) /  CLOCKS_PER_SEC << endl;

    ComponentTree tree(*image);

    cout << "Component Tree done - " <<
         float( clock () - begin_time ) /  CLOCKS_PER_SEC << endl;

    doLinearTopologicalWatershed(*image, tree);

    cout << "Parallel Topological Watershed performed - " <<
            float( clock () - begin_time ) /  CLOCKS_PER_SEC << endl;

    writeImage(destinationImage,format,image->getPixels(),
               image->getWidth(),image->getWidth());

    cout << "Image wrote - " <<
            float( clock () - begin_time ) /  CLOCKS_PER_SEC << endl;

    delete image;
    return 0;
}
