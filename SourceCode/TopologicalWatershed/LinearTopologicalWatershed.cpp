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
    NodeVector nodesFromNeighbor;
    ComponentMapping componentMapping = componentTree.getComponentMapping();
    vector<unsigned int> neighbors = image.getLowerNeighbors(pixelPosition);
    for (auto& neighbor : neighbors)
    {
        nodesFromNeighbor.push_back(componentMapping.at(neighbor));
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
         currentPixel ++)
    {
        Node* node = wDestructible(image,componentTree,currentPixel);
        if (node)
        {
            priority[node->getLevel() - 1].push(currentPixel);
            newGreyLevel[currentPixel] = node->getLevel() - 1;
            newComponent[currentPixel] = node;
        }
    }

    for (ushort k = image.getLowestGreyIntensity();
         k < image.getHighestGreyIntensity(); k++)
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
    vector<unsigned short> dummyPixelArray = { 4,5,6,8,3,4,9,1,7,1,1,1 };
    ImageFourNeighborType image(dummyPixelArray,3,4);
    doLinearTopologicalWatershed(image);

    for (unsigned int i = 0; i < image.getPixels().size(); i++)
    {
        cout << image.getPixels().at(i) << endl;
    }

    return 0;
}
