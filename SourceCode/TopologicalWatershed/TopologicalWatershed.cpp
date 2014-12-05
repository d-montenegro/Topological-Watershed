#include <set>
#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>
#include "Node.h"
#include "TopologicalWatershed.h"
#include "BidimiensionalArrayPartitioner.h"
#include "WDestructibleElementsCollection.h"

using namespace std;

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
    removeIfExistsByPixelPosition(elements,wDestructibleElement.pixelPosition);

    elements.insert(wDestructibleElement);
}

} // anonymous namespace

// global variable to be used by parallel topological watershed
mutex m; // controlling mutex
set<unsigned int> pendingBorderPoints;

void doBorderPointInsertion(unsigned int number)
{
    unique_lock<mutex> lck{m};
    pendingBorderPoints.insert(number);
}

// global variable to be used by parallel topological watershed
set<unsigned int> pendingInnerPoints;


Node* wDestructible(const Image& image, ComponentTree& componentTree,
                    const unsigned int pixelPosition);

void processWDestructibleElement(Image& image, ComponentTree& componentTree,
                                 WDestructibleElement& element,
                                 WDestructibleElementsCollection& wDestructibleElements);

WDestructibleElementsCollection initializeSet(Image& image,
                                        ComponentTree& componentTree,
                                        const Tile& tile);

vector<Tile> divideImageInTiles(Image&, ushort);

void doTopologicalWatershedOnTile(Image& image,
                                  ComponentTree& componentTree,
                                  const Tile& tile,
                                  const Tile pixelsToProcess);

void doTopologicalWatershedOnBorder(Image& image,
                                    ComponentTree& componentTree);

/*
 * Performs the Topological Watershed in cuasi-lineal time
 */
void doLinearTopologicalWatershed(Image& image, ComponentTree& componentTree)
{
    size_t imageSize = image.getPixels().size();
    WDestructibleElementsCollection wDestructibleElements(imageSize);
    for (unsigned int currentPixel = 0; currentPixel < imageSize; currentPixel++)
    {
        Node* node = wDestructible(image,componentTree,currentPixel);
        if (node)
        {
            wDestructibleElements.addElement(WDestructibleElement(currentPixel,node));
        }
    }

    while(!wDestructibleElements.isEmpty())
    {
        WDestructibleElement element = wDestructibleElements.getMinimum();
        processWDestructibleElement(image,componentTree,element,wDestructibleElements);
    }
}

/*
 * Performs the Topological Watershed in parallel
 */
void doParallelTopologicalWatershed(Image& image, ComponentTree& componentTree,
                                    ushort numberOfThreads)
{
    pendingBorderPoints.clear();
    pendingInnerPoints.clear();

    // TODO: check for possible values of variable numberOfThreads!
    vector<Tile>tiles = divideSquareIntoTiles(image.getWidth(),image.getHeight(),
                                              numberOfThreads);

    vector<thread> threadPool;
    for_each(tiles.begin(),tiles.end(),[&] (Tile& tile)
    {
         threadPool.push_back(thread(doTopologicalWatershedOnTile,ref(image),
                                     ref(componentTree),ref(tile),tile));
    });

    // wait for all threads to finish
    for_each(threadPool.begin(),threadPool.end(),[](thread& t) { t.join(); } );

    doTopologicalWatershedOnBorder(image,componentTree);

    while(!pendingInnerPoints.empty())
    {
        pendingBorderPoints.clear();
        threadPool.clear();

        for_each(tiles.begin(),tiles.end(),[&](Tile& tile) {
            Tile intersection;
            set_intersection(tile.begin(),tile.end(),
                          pendingInnerPoints.begin(),pendingInnerPoints.end(),
                          inserter(intersection,intersection.begin()));
            threadPool.push_back(thread(doTopologicalWatershedOnTile,ref(image),
                                     ref(componentTree),ref(tile),intersection));
        });

        // wait for all threads to finish
        for_each(threadPool.begin(),threadPool.end(),[](thread& t) { t.join(); } );

        pendingInnerPoints.clear();

        doTopologicalWatershedOnBorder(image,componentTree);
    }
}

/*
 * Checks if the pixel at pixelPosition is wDestructible or not. If it is,
 * the a pointer to the node this pixel should point to is returned, otherwise,
 * 0 es returned.
 */
Node* wDestructible(const Image& image, ComponentTree& componentTree,
                    const unsigned int pixelPosition)
{
    NodeSet nodesFromNeighbor;
    set<unsigned int> neighbors = image.getLowerNeighbors(pixelPosition);

    for (auto& neighbor : neighbors)
    {
        nodesFromNeighbor.insert(componentTree.getComponentMapping().at(neighbor));
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
                                 WDestructibleElementsCollection& wDestructibleElements)
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
                wDestructibleElements.removeElement(neighbor);
            }
            else
            {
                wDestructibleElements.addElement(WDestructibleElement(neighbor,node));
            }
        }
    }
}

WDestructibleElementsCollection initializeSet(Image& image,
                                        ComponentTree& componentTree,
                                        const Tile& tile)
{
    WDestructibleElementsCollection elements(image.getPixels().size());
    for (auto point : tile)
    {
        Node* node = wDestructible(image,componentTree,point);
        if (node)
        {
            elements.addElement(WDestructibleElement(point,node));
        }
    }

    return elements;
}

void doTopologicalWatershedOnTile(Image& image,
                                  ComponentTree& componentTree,
                                  const Tile& tile,
                                  const Tile pixelsToProcess)
{
    WDestructibleElementsCollection elements = initializeSet(image,componentTree,pixelsToProcess);
    while(!elements.isEmpty())
    {
        WDestructibleElement element = elements.getMinimum();
        set<unsigned int> neighbors = image.getNeighbors(element.pixelPosition);
        if (includes(tile.begin(),tile.end(),neighbors.begin(),neighbors.end()))
        {
            processWDestructibleElement(image,componentTree,element,elements);
        }
        else
        {
            doBorderPointInsertion(element.pixelPosition);
        }
    }
}

void doTopologicalWatershedOnBorder(Image& image,
                                    ComponentTree& componentTree)
{
    WDestructibleElementsCollection elements = initializeSet(image,componentTree,
                                                       pendingBorderPoints);
    while(!elements.isEmpty())
    {
        WDestructibleElement element = elements.getMinimum();
        image.setPixelValue(element.pixelPosition,element.futureNode->getLevel() - 1);
        componentTree.getComponentMapping().at(element.pixelPosition) = element.futureNode;

        for (auto& neighbor : image.getNeighbors(element.pixelPosition))
        {
            if (element.futureNode->getLevel() <= image.getPixels().at(neighbor))
            {
                Node* node = wDestructible(image,componentTree,neighbor);
                if (!node)
                {
                    elements.removeElement(neighbor);
                    pendingInnerPoints.erase(neighbor);
                }
                else
                {
                    if(elements.isPresent(neighbor))
                    {
                        elements.addElement(WDestructibleElement(neighbor,node));
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
