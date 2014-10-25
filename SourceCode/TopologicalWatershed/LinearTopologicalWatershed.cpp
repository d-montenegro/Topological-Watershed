#include <iostream>
#include <set>
#include "Node.h"
#include "ComponentTree.h"
#include "ImageFourNeighborType.h"
#include "WDestructibleElement.h"

namespace
{
// UTILITIES

/*
 * Remove from elements an element by pixelPosition
 */
void removeIfExistsByPixelPosition(set<WDestructibleElement>& elements,
                              unsigned int pixelPosition)
{
    WDestructibleElement elem(0,0);
    for (auto& element : elements)
    {
        if (element.pixelPosition == pixelPosition)
        {
            elem = element;
            break;
        }
    }

    if (elem.futureNode != 0)
    {
        elements.erase(elem);
    }
}

/*
 * Insert or update an element by pixelPosition
 */
void addOrUpdate(set<WDestructibleElement>& elements,
                 const WDestructibleElement& wDestructibleElement)
{
    bool found = false;
    for (auto& element : elements)
    {
        if (element.pixelPosition == wDestructibleElement.pixelPosition)
        {
            element.futureNode = wDestructibleElement.futureNode;
            found = true;
            break;
        }
    }

    if (!found)
    {
        elements.insert(wDestructibleElement);
    }
}

} // anonymous namespace

using namespace std;

/*
 * Checks if the pixel at pixelPosition is wDestructible or not. If it is,
 * the a pointer to the node this pixel should point to is returned, otherwise,
 * 0 es returned.
 */
Node* wDestructible(const Image& image, ComponentTree& componentTree,
                    const unsigned int pixelPosition)
{
    NodeSet nodesFromNeighbor;
    ComponentMapping componentMapping = componentTree.getComponentMapping();
    set<unsigned int> neighbors = image.getLowerNeighbors(pixelPosition);

    for (auto& neighbor : neighbors)
    {
        nodesFromNeighbor.insert(componentMapping.at(neighbor));
    }

    if (nodesFromNeighbor.empty())
    {
        return 0;
    }

    if (1 == nodesFromNeighbor.size())
    {
        return *(nodesFromNeighbor.begin());
    }

    Node* highestFork = componentTree.getHighestFork(nodesFromNeighbor);

    if (!highestFork)
    {
        return componentTree.getMinimum(nodesFromNeighbor);
    }

    if (highestFork->getLevel() <= image.getPixels().at(pixelPosition))
    {
        return highestFork;
    }

    return 0;
}

/*
 * Performs the Topological Watershed in cuasi-lineal time
 */
void doLinearTopologicalWatershed(Image& image, ComponentTree componentTree)
{
    set<WDestructibleElement> wDestructibleElements;
    for (unsigned int currentPixel = 0; currentPixel < image.getPixels().size();
         currentPixel++)
    {
        Node* node = wDestructible(image,componentTree,currentPixel);
        if (node)
        {
            wDestructibleElements.insert(WDestructibleElement(currentPixel,node));
        }
    }

    while(!wDestructibleElements.empty())
    {
        WDestructibleElement element = *wDestructibleElements.begin();
        wDestructibleElements.erase(wDestructibleElements.begin());
        image.setPixelValue(element.pixelPosition,element.futureNode->getLevel() - 1);
        componentTree.getComponentMapping().at(element.pixelPosition) = element.futureNode;

        for (auto& neighbor : image.getNeighbors(element.pixelPosition))
        {
            if (element.futureNode->getLevel() <= image.getPixels().at(neighbor))
            {
                Node* node = wDestructible(image,componentTree,neighbor);
                if (!node)
                {
                    removeIfExistsByPixelPosition(wDestructibleElements, neighbor);
                }
                else
                {
                    addOrUpdate(wDestructibleElements, WDestructibleElement(neighbor, node));
                }
            }
        }
    }
}

/*
 * Local and temporary test for the lineal topological watershed functionality
 */
int main (void)
{
    // Building Component Tree for dummy array
    vector<ushort> dummyPixelArray = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                       0,1,2,11,11,11,4,4,3,3,0,5,11,11,11,3,2,0,
                                       0,3,12,6,7,5,11,7,6,5,7,11,5,7,7,12,4,0,
                                       0,15,7,4,2,3,5,11,11,11,11,5,3,1,7,7,15,0,
                                       0,3,14,7,3,5,11,8,7,8,8,11,5,3,7,14,5,0,
                                       0,1,2,13,11,11,3,2,2,1,2,4,11,11,11,4,3,0,
                                       0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

    ImageFourNeighborType image(dummyPixelArray,18,7);

    doLinearTopologicalWatershed(image, ComponentTree(image));

    for (unsigned int i = 0; i < image.getPixels().size(); i++)
    {
        if (0 == i % image.getWidth())
        {
            cout << endl;
        }
        if (image.getPixels().at(i) < 10)
            cout << "0" << image.getPixels().at(i) << " ";
        else
            cout << image.getPixels().at(i) << " ";

    }
    cout << endl;

    return 0;
}
