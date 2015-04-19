#include <set>
#include <vector>
#include <queue>
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

vector<int> lowerCompletion(const Image& image)
{
    unsigned int size = image.getSize();
    vector<int> lowerCompletion(size,0);
    queue<int> pixelsQueue;

    unsigned int ficticiousPixel = size+1; // should be max
    for(unsigned int i = 0; i < size; i++)
    {
        if(!image.getLowerNeighbors(i).empty())
        {
            pixelsQueue.push(i);
            lowerCompletion.at(i) = -1;
        }
    }

    unsigned int dist = 1;
    pixelsQueue.push(ficticiousPixel);

    while(!pixelsQueue.empty())
    {
        unsigned int pixel = pixelsQueue.front();
        pixelsQueue.pop();

        if(pixel == ficticiousPixel)
        {
            if(!pixelsQueue.empty())
            {
                pixelsQueue.push(ficticiousPixel);
                dist++;
            }
        }
        else
        {
            lowerCompletion.at(pixel) = dist;
            for(auto& neighbor : image.getNeighbors(pixel))
            {
                if(image.at(neighbor) == image.at(pixel) &&
                        lowerCompletion.at(neighbor) == 0)
                {
                    pixelsQueue.push(neighbor);
                    lowerCompletion.at(neighbor) = -1;
                }
            }
        }
    }

    for(unsigned int i = 0; i < size; i++)
    {
        if(lowerCompletion.at(i) != 0)
        {
            lowerCompletion.at(i) = dist * image.at(i) + lowerCompletion.at(i) - 1;
        }
        else
        {
            lowerCompletion.at(i) = dist * image.at(i);
        }
    }

    return lowerCompletion;
}

} // anonymous namespace

enum PIXEL_TYPE
{
    MINIMUM = 0,
    SEPARATING,
    INNER,
    PLATEAU,
    FUTURE_MINIMUM,
    FUTURE_SEPARATING,
    FUTURE_INNER,
    FUTURE_PLATEAU
};

// global variables to be used by parallel topological watershed
mutex m; // controlling mutex
set<unsigned int> pendingBorderPoints;
set<unsigned int> pendingInnerPoints;

void doBorderPointInsertion(unsigned int number)
{
    unique_lock<mutex> lck{m};
    pendingBorderPoints.insert(number);
}

Node* wDestructible(const Image& image, ComponentTree& componentTree,
                    unsigned int pixelPosition, PIXEL_TYPE& type);

void processWDestructibleElement(Image& image, ComponentTree& componentTree,
                                 WDestructibleElement& element,
                                 WDestructibleElementsCollection& wDestructibleElements,
                                 const vector<int>& priority);

WDestructibleElementsCollection initializeSet(Image& image,
                                        ComponentTree& componentTree,
                                        const set<unsigned int> &pixelsToProcess,
                                        const vector<int>& priority);

vector<Tile> divideImageInTiles(Image&, ushort);

void doTopologicalWatershedOnTile(Image& image,
                                  ComponentTree& componentTree,
                                  const Tile& tile,
                                  const set<unsigned int> pixelsToProcess,
                                  const vector<int>& priority);

void doTopologicalWatershedOnBorder(Image& image,
                                    ComponentTree& componentTree,
                                    const vector<int>& priority);

/*
 * Performs the Topological Watershed in cuasi-lineal time
 */
void doLinearTopologicalWatershed(Image& image, ComponentTree& componentTree)
{
    vector<int> priority = lowerCompletion(image);
    WDestructibleElementsCollection wDestructibleElements;

    size_t imageSize = image.getSize();
    for (unsigned int currentPixel = 0; currentPixel < imageSize; currentPixel++)
    {
        PIXEL_TYPE type = MINIMUM;
        Node* node = wDestructible(image,componentTree,currentPixel,type);
        if (node)
        {
            wDestructibleElements.addElement(
                        WDestructibleElement(currentPixel,
                                             image.at(currentPixel),
                                             priority.at(currentPixel),
                                             node,
                                             type == FUTURE_MINIMUM));
        }
    }

    while(!wDestructibleElements.isEmpty())
    {
        WDestructibleElement element = wDestructibleElements.getMinimum();
        processWDestructibleElement(image,componentTree,element,
                                    wDestructibleElements,priority);
    }
}

/*
 * Performs the Topological Watershed in parallel
 */
void doParallelTopologicalWatershed(Image& image, ComponentTree& componentTree,
                                    ushort numberOfThreads)
{
    if(numberOfThreads < 1)
    {
        throw invalid_argument("Invalid number of threads");
    }

    pendingBorderPoints.clear();
    pendingInnerPoints.clear();

    vector<int> priority = lowerCompletion(image);
    vector<Tile>tiles = divideSquareIntoTiles(image.getWidth(),image.getHeight(),
                                              numberOfThreads);

    vector<thread> threadPool;
    for_each(tiles.begin(),tiles.end(),[&] (Tile& tile)
    {
         threadPool.push_back(thread(doTopologicalWatershedOnTile,ref(image),
                                     ref(componentTree),ref(tile),tile.points,
                                     ref(priority)));
    });

    // wait for all threads to finish
    for_each(threadPool.begin(),threadPool.end(),[](thread& t) { t.join(); } );

    doTopologicalWatershedOnBorder(image,componentTree,priority);

    while(!pendingInnerPoints.empty())
    {
        pendingBorderPoints.clear();
        threadPool.clear();

        for_each(tiles.begin(),tiles.end(),[&](Tile& tile) {
            set<unsigned int> intersection;
            set_intersection(tile.points.begin(),tile.points.end(),
                          pendingInnerPoints.begin(),pendingInnerPoints.end(),
                          inserter(intersection,intersection.begin()));
            threadPool.push_back(thread(doTopologicalWatershedOnTile,ref(image),
                                     ref(componentTree),ref(tile),intersection,
                                     ref(priority)));
        });

        // wait for all threads to finish
        for_each(threadPool.begin(),threadPool.end(),[](thread& t) { t.join(); } );

        pendingInnerPoints.clear();

        doTopologicalWatershedOnBorder(image,componentTree, priority);
    }
}

/*
 * Checks if the pixel at pixelPosition is wDestructible or not. If it is,
 * the a pointer to the node this pixel should point to is returned, otherwise,
 * 0 es returned.
 */
Node* wDestructible(const Image& image, ComponentTree& componentTree,
                    unsigned int pixelPosition, PIXEL_TYPE& type)
{
    type = MINIMUM;
    NodeSet nodesFromNeighbor;
    set<unsigned int> neighbors = image.getLowerNeighbors(pixelPosition);

    for (auto& neighbor : neighbors)
    {
        nodesFromNeighbor.insert(componentTree.getComponent(neighbor));
    }

    if (nodesFromNeighbor.empty())
    {
        Node* n = componentTree.getComponent(pixelPosition);
        if(componentTree.isLeaf(n))
        {
            type = MINIMUM;
        }
        else
        {
            type = PLATEAU;
        }
        return 0;
    }

    if (1 == nodesFromNeighbor.size())
    {
        Node* n = *(nodesFromNeighbor.begin());
        if(componentTree.isLeaf(n))
        {
            type = FUTURE_MINIMUM;
        }
        else
        {
            type = FUTURE_PLATEAU;
        }
        return n;
    }

    Node* highestFork = componentTree.getHighestFork(nodesFromNeighbor);

    if (!highestFork)
    {
        Node* n = componentTree.getMinimum(nodesFromNeighbor);
        if(componentTree.isLeaf(n))
        {
            type = FUTURE_MINIMUM;
        }
        else
        {
            type = FUTURE_PLATEAU;
        }
        return n;
    }

    if (highestFork->getLevel() <= image.at(pixelPosition))
    {
        type = FUTURE_SEPARATING;
        return highestFork;
    }

    type = SEPARATING;
    return 0;
}

void processWDestructibleElement(Image& image, ComponentTree& componentTree,
                                 WDestructibleElement& element,
                                 WDestructibleElementsCollection& wDestructibleElements,
                                 const vector<int>& priority)
{
    image.setPixelValue(element.pixelPosition,element.futureNode->getLevel() - 1);
    componentTree.setComponent(element.pixelPosition,element.futureNode);

    for (auto& neighbor : image.getNeighbors(element.pixelPosition))
    {
        if (element.futureNode->getLevel() <= image.at(neighbor))
        {
            PIXEL_TYPE type = MINIMUM;
            Node* node = wDestructible(image,componentTree,neighbor,type);
            if (!node)
            {
                wDestructibleElements.removeElement(neighbor);
            }
            else
            {
                wDestructibleElements.addElement(
                            WDestructibleElement(neighbor,
                                                 image.at(neighbor),
                                                 priority.at(neighbor),
                                                 node,
                                                 type == FUTURE_MINIMUM));
            }
        }
    }
}

WDestructibleElementsCollection initializeSet(Image& image,
                                        ComponentTree& componentTree,
                                        const set<unsigned int>& pixelsToProcess,
                                        const vector<int>& priority)
{
    WDestructibleElementsCollection elements;
    for (auto point : pixelsToProcess)
    {
        PIXEL_TYPE type = MINIMUM;
        Node* node = wDestructible(image,componentTree,point,type);
        if (node)
        {
            elements.addElement(WDestructibleElement(point,image.at(point),
                                                     priority.at(point),node,
                                                     type == FUTURE_MINIMUM));
        }
    }

    return elements;
}

void doTopologicalWatershedOnTile(Image& image,
                                  ComponentTree& componentTree,
                                  const Tile& tile,
                                  const set<unsigned int> pixelsToProcess,
                                  const vector<int>& priority)
{
    WDestructibleElementsCollection elements = initializeSet(
                image,componentTree,pixelsToProcess,priority);
    while(!elements.isEmpty())
    {
        WDestructibleElement element = elements.getMinimum();
        set<unsigned int> neighbors = image.getNeighbors(element.pixelPosition);

        unsigned int raw = 0, col = 0;
        bool isBorder = false;
        for(auto& n : neighbors)
        {
            image.getCoordinates(n,raw,col);

            if(tile.begin.first > raw || raw > tile.end.first ||
               tile.begin.second > col || col > tile.end.second)
            {
                isBorder = true;
                break;
            }
        }
        image.getCoordinates(element.pixelPosition,raw,col);
        if (!isBorder)
        {
            processWDestructibleElement(image,componentTree,element,elements,priority);
        }
        else
        {
            doBorderPointInsertion(element.pixelPosition);
        }
    }
}

void doTopologicalWatershedOnBorder(Image& image,
                                    ComponentTree& componentTree,
                                    const vector<int>& priority)
{
    WDestructibleElementsCollection elements = initializeSet(image,componentTree,
                                                             pendingBorderPoints,
                                                             priority);
    while(!elements.isEmpty())
    {
        WDestructibleElement element = elements.getMinimum();
        image.setPixelValue(element.pixelPosition,element.futureNode->getLevel() - 1);
        componentTree.setComponent(element.pixelPosition,element.futureNode);

        for (auto& neighbor : image.getNeighbors(element.pixelPosition))
        {
            if (element.futureNode->getLevel() <= image.at(neighbor))
            {
                PIXEL_TYPE type = MINIMUM;
                Node* node = wDestructible(image,componentTree,neighbor,type);
                if (!node)
                {
                    elements.removeElement(neighbor);
                    pendingInnerPoints.erase(neighbor);
                }
                else
                {
                    if(elements.isPresent(neighbor))
                    {
                        elements.addElement(
                                    WDestructibleElement(neighbor,image.at(neighbor),
                                                         priority.at(neighbor),node,
                                                         type == FUTURE_MINIMUM));
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


/**************************** NAIVE IMPLEMENTATION **************************/
bool isWDestructible(const Image& image, unsigned int pixelPosition)
{
    set<unsigned int> neighbors = image.getLowerNeighbors(pixelPosition);
    if(neighbors.empty())
    {
        return false;
    }
    if(neighbors.size() == 1)
    {
        return true;
    }

    vector<unsigned int> reachablePoints;
    reachablePoints.push_back(*neighbors.begin());
    neighbors.erase(neighbors.begin());
    unsigned int index = 0;

    while(!neighbors.empty() && index < reachablePoints.size())
    {
        for(auto& neighborPosition : image.getNeighbors(reachablePoints.at(index)))
        {
            if(image.at(neighborPosition) < image.at(pixelPosition)
                    && reachablePoints.end() ==
                    find(reachablePoints.begin(),reachablePoints.end(),neighborPosition))
            {
                reachablePoints.push_back(neighborPosition);
                neighbors.erase(neighborPosition);
            }
        }
        index++;
    }

    return neighbors.empty();
}

void doNaiveTopologicalWatershed(Image& image)
{
    bool someChange = false;
    unsigned int totalPixels = image.getSize();
    do
    {
        someChange = false;
        for(unsigned int i = 0; i < totalPixels; i++)
        {
            if(isWDestructible(image,i))
            {
                image.decrementPixel(i);
                someChange = true;
            }
        }
    } while(someChange);
}

