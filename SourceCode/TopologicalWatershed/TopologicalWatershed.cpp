#include <set>
#include <vector>
#include <queue>
#include <algorithm>
#include <thread>
#include <mutex>
#include <cassert>

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

vector<short> lowerCompletion(const Image& image)
{
    unsigned int size = image.getSize();
    vector<short> lowerCompletion(size,0);
    queue<int> queue2;

    unsigned int ficticiousPixel = size+1; // should be max
    for(unsigned int i = 0; i < size; i++)
    {
        if(!image.getLowerNeighbors(i).empty())
        {
            queue2.push(i);
            lowerCompletion.at(i) = -1;
        }
    }

    unsigned int dist = 1;
    queue2.push(ficticiousPixel);

    while(!queue2.empty())
    {
        unsigned int p = queue2.front();
        queue2.pop();

        if(p == ficticiousPixel)
        {
            if(!queue2.empty())
            {
                queue2.push(ficticiousPixel);
                dist++;
            }
        }
        else
        {
            lowerCompletion.at(p) = dist;
            for(auto& q : image.getNeighbors(p))
            {
                if(image.at(q) == image.at(p) &&
                        lowerCompletion.at(q) == 0)
                {
                    queue2.push(q);
                    lowerCompletion.at(q) = -1;
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

// global variable to be used by parallel topological watershed
mutex m; // controlling mutex
set<unsigned int> pendingBorderPoints;
vector<short> lc;

void doBorderPointInsertion(unsigned int number)
{
    unique_lock<mutex> lck{m};
    pendingBorderPoints.insert(number);
}

// global variable to be used by parallel topological watershed
set<unsigned int> pendingInnerPoints;


Node* wDestructible(const Image& image, ComponentTree& componentTree,
                    unsigned int pixelPosition, bool& isSeparating);

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
    size_t imageSize = image.getSize();
    lc = lowerCompletion(image);
    WDestructibleElementsCollection wDestructibleElements(imageSize);
    for (unsigned int currentPixel = 0; currentPixel < imageSize; currentPixel++)
    {
        bool isSeparating = false;
        Node* node = wDestructible(image,componentTree,currentPixel,isSeparating);
        if (node)
        {
            wDestructibleElements.addElement(WDestructibleElement(currentPixel,
                                                                  image.at(currentPixel),
                                                                  lc.at(currentPixel),
                                                                  node,
                                                                  isSeparating && node->getChilds().empty()));
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

    lc = lowerCompletion(image);

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
                    unsigned int pixelPosition, bool& isSeparating)
{
    isSeparating = false;
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
        isSeparating = true;
        return componentTree.getMinimum(nodesFromNeighbor);
    }

    if (highestFork->getLevel() <= image.at(pixelPosition))
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
        if (element.futureNode->getLevel() <= image.at(neighbor))
        {
            bool isSeparating = false;
            Node* node = wDestructible(image,componentTree,neighbor,isSeparating);
            if (!node)
            {
                wDestructibleElements.removeElement(neighbor);
            }
            else
            {
                wDestructibleElements.addElement(WDestructibleElement(neighbor,
                                                                      image.at(neighbor),
                                                                      lc.at(neighbor),
                                                                      node,
                                                                      isSeparating && node->getChilds().empty()));
            }
        }
    }
}

WDestructibleElementsCollection initializeSet(Image& image,
                                        ComponentTree& componentTree,
                                        const Tile& tile)
{
    WDestructibleElementsCollection elements(image.getSize());
    for (auto point : tile)
    {
        bool isSeparating = false;
        Node* node = wDestructible(image,componentTree,point,isSeparating);
        if (node)
        {
            elements.addElement(WDestructibleElement(point,image.at(point),
                                                     lc.at(point),node,
                                                     isSeparating && node->getChilds().empty()));
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
            if (element.futureNode->getLevel() <= image.at(neighbor))
            {
                bool isSeparating = false;
                Node* node = wDestructible(image,componentTree,neighbor,isSeparating);
                if (!node)
                {
                    elements.removeElement(neighbor);
                    pendingInnerPoints.erase(neighbor);
                }
                else
                {
                    if(elements.isPresent(neighbor))
                    {
                        elements.addElement(WDestructibleElement(neighbor,image.at(neighbor),
                                                                 lc.at(neighbor),node,
                                                                 isSeparating && node->getChilds().empty()));
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

