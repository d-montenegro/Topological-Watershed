#ifndef __COMPONENTTREE_H_INCLUDED__
#define __COMPONENTTREE_H_INCLUDED__

#include <map>
#include <string>
#include <vector>

#include "Image.h"
#include "Node.h"

using namespace std;

typedef map<unsigned int, Node*> ComponentMapping;

class ComponentTree
{
public:
    ComponentTree(const Image& image);
    ~ComponentTree();

    Node* getMinimum(const NodeSet& nodes) const;
    Node* getHighestFork(const NodeSet& nodes) const;

    unsigned int getTotalNodes() const { return totalNodes; }
    Node* getRoot() const { return root; }
    ComponentMapping getComponentMapping() const { return componentMapping; }

private:
    Node* getBinaryLeastCommonAncestor(Node *node1, Node *node2) const;

    unsigned int totalNodes;
    ComponentMapping componentMapping;
    Node* root;
};

#endif // __COMPONENTTREE_H_INCLUDED__
