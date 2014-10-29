#include <set>
#include <vector>
#include <algorithm>
#include <thread>
#include "Node.h"
#include "WDestructibleElement.h"
#include "LinearTopologicalWatershed.h"

namespace
{
// UTILITIES

/*
 * Remove from elements an element by pixelPosition
 */
void removeIfExistsByPixelPosition(set<WDestructibleElement>& elements,
                              unsigned int pixelPosition)
{
    set<WDestructibleElement>::iterator it =
            find_if(elements.begin(),elements.end(),
            [&](const WDestructibleElement& e)
            {
                return e.pixelPosition == pixelPosition;
            });
    if (it != elements.end())
    {
        elements.erase(it);
    }
}

/*
 * Insert or update an element by pixelPosition
 */
void addOrUpdate(set<WDestructibleElement>& elements,
                 const WDestructibleElement& wDestructibleElement)
{
    set<WDestructibleElement>::iterator it =
            find(elements.begin(),elements.end(),wDestructibleElement);
    if (it != elements.end())
    {
        it->futureNode = wDestructibleElement.futureNode;
    }
    else
    {
        elements.insert(wDestructibleElement);
    }
}

} // anonymous namespace

using Tile = set<unsigned int>;
using namespace std;

// global variable to be used by parallel topological watershed
set<unsigned int> pendingBorderPoints;

// global variable to be used by parallel topological watershed
set<unsigned> pendingInnerPoints;

/*******************************************************************************
                   BEGIN LINEAL TOPOLOGICAL WATERSHED
*******************************************************************************/
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

void processWDestructibleElement(Image& image, ComponentTree& componentTree,
                                 WDestructibleElement& element,
                                 set<WDestructibleElement>& wDestructibleElements)
{
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

/*
 * Performs the Topological Watershed in cuasi-lineal time
 */
void doLinearTopologicalWatershed(Image& image, ComponentTree& componentTree)
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
        processWDestructibleElement(image,componentTree,element,wDestructibleElements);
    }
}

/*******************************************************************************
                   BEGIN PARALLEL TOPOLOGICAL WATERSHED
*******************************************************************************/

set<WDestructibleElement> initializeSet(Image& image,
                                        ComponentTree& componentTree,
                                        const Tile& tile)
{
    set<WDestructibleElement> elements;
    for (auto point : tile)
    {
        Node* node = wDestructible(image,componentTree,point);
        if (node)
        {
            elements.insert(WDestructibleElement(point,node));
        }
    }

    return elements;
}

void doTopologicalWatershedOnTile(Image& image,
                                  ComponentTree& componentTree,
                                  const Tile& tile)
{
    set<WDestructibleElement> elements = initializeSet(image,componentTree,tile);
    while(!elements.empty())
    {
        WDestructibleElement element = *elements.begin();
        elements.erase(elements.begin());
        set<unsigned int> neighbors = image.getNeighbors(element.pixelPosition);
        if (includes(tile.begin(),tile.end(),neighbors.begin(),neighbors.end()))
        {
            pendingBorderPoints.insert(element.pixelPosition);
        }
        else
        {
            processWDestructibleElement(image,componentTree,element,elements);
        }
    }
}

void doTopologicalWatershedOnBorder(Image& image,
                                    ComponentTree& componentTree)
{
    set<WDestructibleElement> elements = initializeSet(image,componentTree,
                                                       pendingBorderPoints);
    while(!elements.empty())
    {
        WDestructibleElement element = *elements.begin();
        elements.erase(elements.begin());

        image.setPixelValue(element.pixelPosition,element.futureNode->getLevel() - 1);
        componentTree.getComponentMapping().at(element.pixelPosition) = element.futureNode;

        for (auto& neighbor : image.getNeighbors(element.pixelPosition))
        {
            if (element.futureNode->getLevel() <= image.getPixels().at(neighbor))
            {
                Node* node = wDestructible(image,componentTree,neighbor);
                if (!node)
                {
                    removeIfExistsByPixelPosition(elements, neighbor);
                    pendingInnerPoints.erase(neighbor);
                }
                else
                {
                    set<WDestructibleElement>::iterator it =
                            find_if(elements.begin(),elements.end(),
                            [&](const WDestructibleElement& e)
                            {
                                return e.pixelPosition == neighbor;
                            });
                    if(it != elements.end())
                    {
                        it->futureNode = node;
                    }
                    else
                    {
                        pendingInnerPoints.insert(neighbor);
                    }
                }
            }
        }
    }
}

vector<Tile> divideImageInTiles(Image&, ushort)
{
    return vector<Tile>();
}

void doParallelTopologicalWatershed(Image& image, ComponentTree& componentTree,
                                    ushort numberOfThreads)
{
    // TODO: check for possible values of number of threads!
    vector<Tile>tiles = divideImageInTiles(image, numberOfThreads);
    vector<thread> threadPool;
    for(ushort i = 0; i < numberOfThreads; i++)
    {
        threadPool.push_back(thread(doTopologicalWatershedOnTile,ref(image),ref(componentTree),
                                    ref(tiles.at(i))));
    }

    for_each(threadPool.begin(),threadPool.end(),[](thread& t) { t.join(); } );
}

