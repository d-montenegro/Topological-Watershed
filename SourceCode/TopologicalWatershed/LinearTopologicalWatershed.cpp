#include <iostream>
#include <string>
#include <queue>
#include <limits>

#include <assert.h>

#include "Node.h"
#include "ComponentTree.h"
#include "ImageFourNeighborType.h"


using namespace std;

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

void doLinearTopologicalWatershed(Image& image)
{
    map<ushort, set<unsigned int> > priority;
    map<unsigned int, ushort> newGreyLevel;
    map<unsigned int, Node*> newComponent;
    ComponentTree componentTree(image);
    for (unsigned int currentPixel = 0; currentPixel < image.getPixels().size();
         currentPixel++)
    {
        Node* node = wDestructible(image,componentTree,currentPixel);
        if (node)
        {
            ushort newLevel = node->getLevel() > 0 ? node->getLevel() - 1 : 0;
            priority[newLevel].insert(currentPixel);
            newGreyLevel[currentPixel] = newLevel;
            newComponent[currentPixel] = node;
        }
    }

    for (ushort k = image.getLowestGreyIntensity();
         k <= image.getHighestGreyIntensity(); k++)
    {
        while (!priority[k].empty())
        {
            unsigned int pixelPosition = *priority[k].begin();
            priority[k].erase(priority[k].begin());
            if (newGreyLevel[pixelPosition] == k)
            {
                image.setPixelValue(pixelPosition,k);
                componentTree.getComponentMapping().at(pixelPosition) = newComponent.at(pixelPosition);
                assert(componentTree.getComponentMapping().at(pixelPosition) == newComponent.at(pixelPosition));
                for (auto& neighbor : image.getNeighbors(pixelPosition))
                {
                    if (k < image.getPixels().at(neighbor))
                    {
                        Node* node = wDestructible(image,componentTree,neighbor);
                        if (!node)
                        {
                            newGreyLevel[neighbor] = std::numeric_limits<ushort>::max();
                        }
                        else
                        {
                            ushort newLevel = node->getLevel() > 0 ? node->getLevel() - 1 : 0;
                            if (newGreyLevel[neighbor] != newLevel)
                            {
                                priority[newLevel].insert(neighbor);
                                newGreyLevel[neighbor] = newLevel;
                                newComponent[neighbor] = node;
                            }
                        }
                    }
                }
            }
        }
    }
}

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
    doLinearTopologicalWatershed(image);

    for (unsigned int i = 0; i < image.getPixels().size(); i++)
    {
        if (0 == i % image.getWidth())
        {
            cout << endl;
        }
        cout << image.getPixels().at(i) << " ";

    }
    cout << endl;

    return 0;
}
