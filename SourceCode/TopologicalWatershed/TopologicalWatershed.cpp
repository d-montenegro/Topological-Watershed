#include <iostream>
#include <string>

#include "ComponentTree.h"
#include "ImageFourNeighborType.h"

int main (void)
{
    // Building Component Tree for dummy array
    vector<unsigned short> dummyPixelArray = { 4,5,6,8,3,4,9,1,7 };
    ImageFourNeighborType image(dummyPixelArray,3,3);
    ComponentTree tree(image);

    cout << sizeof(tree) << endl;
    return 0;
}
