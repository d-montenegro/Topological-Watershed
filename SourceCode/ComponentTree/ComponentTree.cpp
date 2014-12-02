#include <iostream>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <limits>

#include "ComponentTree.h"
#include "DisjointSetCollection.h"

using namespace std;

namespace
{

struct ArraySorter
{
    vector<ushort> arr;
    ArraySorter(const vector<ushort>& arr) : arr(arr) {}

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
vector<unsigned int> SpecialSort(const vector<ushort>& arr)
{
    vector<unsigned int> result(arr.size());

    iota(result.begin(), result.end(), 0);

    sort(result.begin(), result.end(), ArraySorter(arr));

    return result;
}

void deleteTree(Node* root)
{
    for (auto& node : root->getChilds())
    {
        deleteTree(node);
    }
    delete root;
}

} // namespace

/*
 * Constructor. Builds component tree
 */
ComponentTree::ComponentTree(const Image& image) : totalNodes(0),
    componentMapping(),root(0), lcaSolver(0)

{
    buildComponentTree(image);
}

ComponentTree::~ComponentTree()
{
    delete lcaSolver;
    deleteTree(root);
}

// TODO: split this method in several ones
void ComponentTree::buildComponentTree(const Image& image)
{
    // stores all the partial trees being made during component tree building
    DisjointSetCollection partialTrees;

    // stores the root of each partial tree
    vector<unsigned int> partialTreeRoot;

    // stores the canonical element of each partial tree
    DisjointSetCollection canonicalElements;
    vector<Node*> nodes;

    vector<ushort> pixels = image.getPixels();
    unsigned int totalPixels = pixels.size();
    totalNodes = totalPixels;
    for (unsigned int pixelPosition = 0; pixelPosition < totalPixels;
         pixelPosition++)
    {
        // create a node for each pixel
        nodes.push_back(new Node(pixels.at(pixelPosition) + 1));

        // initialize partial trees
        partialTreeRoot.push_back(pixelPosition);
        partialTrees.addNewSet(pixelPosition);
        canonicalElements.addNewSet(pixelPosition);
    }

    set<unsigned int> alreadyProcessed;
    vector<unsigned int> orderedPixels = SpecialSort(image.getPixels());
    for (auto& currentPixel : orderedPixels)
    {
        // mark pixel as processed
        alreadyProcessed.insert(currentPixel);

        // Get the partial tree the current pixel belongs to
        unsigned int currentPartialTree = partialTrees.find(currentPixel);

        // Get the root of that partial tree
        unsigned int localRoot = partialTreeRoot.at(currentPartialTree);

        // Get the node of that root
        unsigned int currentNode = canonicalElements.find(localRoot);

        // Iterate neighbors with lowest grey level
        set<unsigned int> neighbors = image.getLowerOrEqualNeighbors(currentPixel);
        for (auto& currentNeighbor :  neighbors)
        {
            if (alreadyProcessed.find(currentNeighbor) == alreadyProcessed.end())
            {
                continue;
            }

            // partial tree of the neighbor
            unsigned int neighborPartialTree = partialTrees.find(currentNeighbor);

            // Get the root of that partial tree
            unsigned int localRoot = partialTreeRoot.at(neighborPartialTree);

            // Get the node of that root
            unsigned int neighborNode = canonicalElements.find(localRoot);

            // If not already linked, we need to link them now.
            if (currentNode != neighborNode)
            {
                if (nodes.at(currentNode)->getLevel() ==
                        nodes.at(neighborNode)->getLevel())
                {
                    /* Both nodes have the same level, so we need to delete one
                      of them and set the parent of all childs (the ones from
                      currentNode and the ones from the neighbor) one of them.
                     */
                    unsigned int tempNode =
                            canonicalElements.linkSets(currentNode, neighborNode);
                    if (tempNode == currentNode)
                    {

                        nodes.at(currentNode)->addChilds
                                (nodes.at(neighborNode)->getChilds());
                        delete nodes.at(neighborNode);
                        nodes.at(neighborNode) = nodes.at(currentNode);
                    }
                    else
                    {
                        nodes.at(neighborNode)->addChilds
                                (nodes.at(currentNode)->getChilds());
                        delete nodes.at(currentNode);
                        nodes.at(currentNode) = nodes.at(neighborNode);
                    }

                    totalNodes--;
                    /* updating current node the be the canonical element after
                     linking, to keep comparing with the other neighbors */
                    currentNode = tempNode;
                }
                else
                {
                    // currentNode is the father of neighbor node
                    nodes.at(currentNode)->addChild(nodes.at(neighborNode));
                }

                /* both partial trees now should be linked, and the root of
                   the new partial tree is the currentNode */
                currentPartialTree = partialTrees.
                        linkSets(currentPartialTree,neighborPartialTree);
                partialTreeRoot.at(currentPartialTree) = currentNode;
            }
        }
    }

    for (unsigned int pixelPosition = 0; pixelPosition < totalPixels;
         pixelPosition++)
    {
        componentMapping[pixelPosition] = nodes.at(canonicalElements.find(pixelPosition));
    }

    // Lets get the root node of the tree.

    // First get the canonical element of any pixel
    unsigned int node = canonicalElements.find(0);

    // Then get the partial tree of that canonical element
    unsigned int partialt = partialTrees.find(node);

    // Get the root of that partial tree
    unsigned int rootIndex = partialTreeRoot.at(partialt);
    root = nodes.at(rootIndex);
    lcaSolver = new LCASolver(root);
}

Node* ComponentTree::getMinimum(const NodeSet& nodes) const
{
    if (nodes.size() < 2)
    {
        throw invalid_argument
                ("To calculate minumum we need at least two nodes");
    }
    Node* minLevelNode = *nodes.begin();
    for (auto& node: nodes)
    {
        if (node->getLevel() < minLevelNode->getLevel())
        {
            minLevelNode = node;
        }
    }

    return minLevelNode;
}

Node* ComponentTree::getHighestFork(const NodeSet& nodes) const
{
    if (nodes.size() < 2)
    {
        throw invalid_argument
                ("To calculate highest fork we need at least two nodes");
    }
    Node* minLevelNode = getMinimum(nodes);

    Node* highestFork = minLevelNode;
    for (auto& node : nodes)
    {
        if (node != minLevelNode && node != highestFork)
        {
            Node* blca = lcaSolver->getLCA(node,highestFork);
            if (blca && blca != node && blca != highestFork)
            {
                // then they are separated
                highestFork = blca;
            }
        }
    }

    if (highestFork != minLevelNode)
    {
        return highestFork;
    }
    return 0;
}
