#include <iostream>

#include <algorithm>
#include <vector>

#include "ComponentTree.h"
#include "DisjointSetCollection.h"

using namespace std;

namespace
{

struct ArraySorter
{
    const vector<unsigned int> arr;
    ArraySorter(const vector<unsigned int>& arr) : arr(arr) {}

    bool operator() (unsigned int a, unsigned int b) const
    {
        return (arr[a] < arr[b]);
    }
};

/*
 * Returns an array of the same size as 'arr' that contains the indexes of arr
 * sorted increasingly. Example:
 * If input is [3,5,4,1], the output will be [3,0,2,1] because at position 3 is
 * the lowest value (1), at position 0 is the next lowest value (3) and so on.
 */
vector<unsigned int> SpecialSort (const vector<unsigned int>& arr)
{
    vector<unsigned int> result;

    for (unsigned int i = 0; i < arr.size(); i++)
    {
        result.push_back(i);
    }

    sort(result.begin(), result.end(), ArraySorter(arr));

    return result;
}

} // namespace

/*
 * Constructor. Builds component tree
 */
ComponentTree::ComponentTree(const Image& image)
{
    // stores all the partial trees being made during component tree building
    DisjointSetCollection partialTrees;

    // stores the root of each partial tree
    vector<unsigned int> partialTreeRoot;

    // stores the canonical element of each partial tree
    DisjointSetCollection canonicalElements;

    unsigned int totalPixels = image.getPixels().size();
    for (unsigned int pixelPosition = 0; pixelPosition < totalPixels;
         pixelPosition++)
    {
        // create a node for each pixel
        nodes.push_back(new Node(image.getPixels().at(pixelPosition)));

        // initialize partial trees
        partialTreeRoot.push_back(pixelPosition);
        partialTrees.addNewSet(pixelPosition);
        canonicalElements.addNewSet(pixelPosition);
    }

    vector<unsigned int> orderedPixels = SpecialSort(image.getPixels());
    for (unsigned int pixelPosition = 0; pixelPosition < totalPixels;
         pixelPosition++)
    {
        unsigned int currentPixel = orderedPixels.at(pixelPosition);

        // Get the partial tree the current pixel belongs to
        unsigned int currentPartialTree = partialTrees.find(currentPixel);

        // Get the Node of the partial tree root current pixel belongs to
        int currentNode = canonicalElements.find(partialTreeRoot.at(currentPartialTree));

        // Iterate neighbors with lowest grey level
        vector<unsigned int> neighbors = image.getLowerOrEqualNeighbors(currentPixel);
        for (unsigned int neighborPosition = 0; neighborPosition < neighbors.size();
             neighborPosition++)
        {
            unsigned int currentNeighbor = neighbors.at(neighborPosition);

            // partial tree of the neighbor
            unsigned int neighborPartialTree = partialTrees.find(currentNeighbor);

            // Get the Node of the partial tree root current pixeneighborl belongs to
            int neighborNode = canonicalElements.
                    find(partialTreeRoot.at(neighborPartialTree));

            if (currentNode != neighborNode)
            {
                // Not already linked. Link now.
                if (nodes.at(currentNode)->getLevel() ==
                        nodes.at(neighborNode)->getLevel())
                {
                    unsigned int tempNode =
                            canonicalElements.linkSets(currentNode, neighborNode);
                    if (tempNode == currentNode)
                    {
                        nodes.at(currentNode)->addChilds
                                (nodes.at(neighborNode)->getChilds());
                    }
                    else
                    {
                        nodes.at(neighborNode)->addChilds
                                (nodes.at(currentNode)->getChilds());
                    }
                    delete nodes.at(neighborNode);
                    nodes.at(neighborNode) = nodes.at(currentNode);
                    currentNode = tempNode;
                }
                else
                {
                    nodes.at(currentNode)->addChilds(nodes.at(neighborNode)->getChilds());
                }
                currentPartialTree = partialTrees.
                        linkSets(currentPartialTree,neighborPartialTree);
                partialTreeRoot.at(currentPartialTree) = currentNode;
            }
        }

        for (unsigned int pixelPosition = 0; pixelPosition < totalPixels;
             pixelPosition++)
        {
            componentMapping[pixelPosition] = nodes.at(canonicalElements.find(pixelPosition));
        }
    }
}

ComponentTree::~ComponentTree()
{
    for(unsigned int i=0; i < nodes.size(); i++)
    {
        if (nodes[i])
        {
            delete nodes[i];
        }
    }
}

Node* ComponentTree::getHighestFork(const NodeVector& nodes)
{
    return 0;
}

Node* ComponentTree::getMinimumNode(const NodeVector& nodes)
{
    return 0;
}

Node* ComponentTree::getBinaryLeastCommonAncestor(Node* node1, Node* node2)
{
    return 0;
}





