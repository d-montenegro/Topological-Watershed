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
    Node* getHighestFork(const NodeVector& nodes);
    Node* getMinimumNode(const NodeVector& nodes);

    ComponentMapping getComponentMapping() { return componentMapping; }
private:
    Node* getBinaryLeastCommonAncestor(Node *node1, Node *node2);

    ComponentMapping componentMapping;
    NodeVector nodes;
    Node* root;
};

#endif // __COMPONENTTREE_H_INCLUDED__
