#include <iostream>

#include <algorithm>
#include <vector>
#include <stdexcept>

#include "ComponentTree.h"
#include "DisjointSetCollection.h"

using namespace std;

namespace
{

struct ArraySorter
{
    const vector<ushort> arr;
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
vector<ushort> SpecialSort (const vector<ushort>& arr)
{
    vector<ushort> result;

    for (unsigned int i = 0; i < arr.size(); i++)
    {
        result.push_back(i);
    }

    sort(result.begin(), result.end(), ArraySorter(arr));

    return result;
}

// UTILITIES
vector<Node*> eulerTour;
map<Node*,unsigned int> representatives;

void calculateEulerTour(Node* root)
{
    eulerTour.push_back(root);
    for (auto& child : root->getChilds())
    {
        calculateEulerTour(child);
        eulerTour.push_back(root);
    }
}

void calculateRepresentatives(Node* root)
{
    unsigned int pos = 0;
    for (auto& node : eulerTour)
    {
        if (node == root)
        {
            representatives[root] = pos;
            break;
        }
        pos++;
    }
    for (auto& child : root->getChilds())
    {
        calculateRepresentatives(child);
    }

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
ComponentTree::ComponentTree(const Image& image) : componentMapping(),root(0),
    totalNodes(0)
{
    // stores all the partial trees being made during component tree building
    DisjointSetCollection partialTrees;

    // stores the root of each partial tree
    vector<unsigned int> partialTreeRoot;

    // stores the canonical element of each partial tree
    DisjointSetCollection canonicalElements;
    NodeVector nodes;

    unsigned int totalPixels = image.getPixels().size();
    totalNodes = totalPixels;
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

    set<unsigned int> alreadyProcessed;
    vector<ushort> orderedPixels = SpecialSort(image.getPixels());
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
        vector<unsigned int> neighbors = image.getLowerOrEqualNeighbors(currentPixel);
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
                    nodes.at(currentNode)->addChilds(nodes.at(neighborNode));
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

    calculateEulerTour(root);
    calculateRepresentatives(root);
}

ComponentTree::~ComponentTree()
{
    deleteTree(root);
}

Node* ComponentTree::getMinimum(const NodeVector& nodes) const
{
    if (nodes.size() < 2)
    {
        throw invalid_argument
                ("To calculate minumum we need at least two nodes");
    }
    Node* minLevelNode = nodes.at(0);
    for (auto& node: nodes)
    {
        if (node->getLevel() < minLevelNode->getLevel())
        {
            minLevelNode = node;
        }
    }

    return minLevelNode;
}

Node* ComponentTree::getHighestFork(const NodeVector& nodes) const
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
        if (node != minLevelNode)
        {
            Node* blca = getBinaryLeastCommonAncestor(node,highestFork);
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

Node* ComponentTree::getBinaryLeastCommonAncestor(Node* node1, Node* node2) const
{
    if (!node1)
    {
        throw invalid_argument
                ("Null node1 to calcula binary least common ancestor");
    }

    if (!node2)
    {
        throw invalid_argument
                ("Null node2 to calcula binary least common ancestor");
    }

    unsigned int node1Representative = representatives[node1];
    unsigned int node2Representative = representatives[node2];

    if (node1Representative > node2Representative)
    {
        // lets do a swap
        unsigned int temp = node1Representative;
        node1Representative = node2Representative;
        node2Representative = temp;
    }

    Node* blca = eulerTour.at(node1Representative);
    for (unsigned int index = node1Representative + 1;
         index <= node2Representative; index++)
    {
        if (eulerTour.at(index)->getLevel() > blca->getLevel())
        {
            blca = eulerTour.at(index);
        }
    }

    return blca;
}





