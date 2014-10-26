#ifndef __COMPONENTTREE_H_INCLUDED__
#define __COMPONENTTREE_H_INCLUDED__

#include <map>
#include <stdexcept>

#include "Image.h"
#include "Node.h"

using namespace std;

typedef map<unsigned int, Node*> ComponentMapping;

class ComponentTree
{
public:
    ComponentTree(const Image& image);
    ComponentTree(const ComponentTree&) : totalNodes(0), componentMapping(),
        root(0)
    {
        throw std::runtime_error("Unimplemented yet!!");
    }

    ComponentTree& operator= (const ComponentTree&)
    {
        throw std::runtime_error("Unimplemented yet!!");
    }

    ~ComponentTree();

    Node* getMinimum(const NodeSet& nodes) const;
    Node* getHighestFork(const NodeSet& nodes) const;

    unsigned int getTotalNodes() const { return totalNodes; }
    Node* getRoot() const { return root; }
    ComponentMapping& getComponentMapping() { return componentMapping; }

private:
    void buildComponentTree(const Image& image);
    Node* getBinaryLeastCommonAncestor(Node *node1, Node *node2) const;

    unsigned int totalNodes;
    ComponentMapping componentMapping;
    Node* root;
};

#endif // __COMPONENTTREE_H_INCLUDED__
