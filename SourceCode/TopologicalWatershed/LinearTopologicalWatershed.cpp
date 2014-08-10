#include <iostream>
#include <string>
#include <queue>
#include <limits>

#include "Node.h"
#include "ComponentTree.h"
#include "ImageFourNeighborType.h"


using namespace std;

Node* wDestructible(const Image& image, const ComponentTree& componentTree,
                    const unsigned int pixelPosition)
{
    NodeSet nodesFromNeighbor;
    ComponentMapping componentMapping = componentTree.getComponentMapping();
    set<unsigned int> neighbors = image.getLowerNeighbors(pixelPosition);
    if (0 == neighbors.size())
    {
        return 0;
    }
    for (auto& neighbor : neighbors)
    {
        nodesFromNeighbor.insert(componentMapping.at(neighbor));
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
    map<ushort, queue<unsigned int> > priority;
    map<unsigned int, ushort> newGreyLevel;
    map<unsigned int, Node*> newComponent;
    ComponentTree componentTree(image);
    for (unsigned int currentPixel = 0; currentPixel < image.getPixels().size();
         currentPixel++)
    {
        Node* node = wDestructible(image,componentTree,currentPixel);
        if (node)
        {
            cout << node->getLevel() << endl;
            priority[node->getLevel()].push(currentPixel);
            newGreyLevel[currentPixel] = node->getLevel();
            newComponent[currentPixel] = node;
        }
    }

    for (ushort k = image.getLowestGreyIntensity();
         k <= image.getHighestGreyIntensity(); k++)
    {
        while (!priority[k].empty())
        {
            unsigned int pixelPosition = priority[k].front();
            priority[k].pop();
            if (newGreyLevel[pixelPosition] == k)
            {
                image.getPixels().at(pixelPosition) = k;
                componentTree.getComponentMapping().at(pixelPosition) =
                        newComponent.at(pixelPosition);
                for (auto& neighbor : image.getNeighbors(pixelPosition))
                {
                    if (k < image.getPixels().at(neighbor))
                    {
                        Node* node = wDestructible(image,componentTree,neighbor);
                        if (!node)
                        {
                            newGreyLevel[neighbor] = std::numeric_limits<ushort>::max();
                        }
                        else if (newGreyLevel[neighbor] != node->getLevel() - 1)
                        {
                            priority[node->getLevel() - 1].push(neighbor);
                            newGreyLevel[neighbor] = node->getLevel() - 1;
                            newComponent[neighbor] = node;
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
    vector<unsigned short> dummyPixelArray = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
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
