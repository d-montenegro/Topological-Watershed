#ifndef __COMPONENTTREE_H_INCLUDED__
#define __COMPONENTTREE_H_INCLUDED__

#include <map>
#include <stdexcept>

#include "Image.h"
#include "Node.h"
#include "LCASolver.h"

using namespace std;

typedef map<unsigned int, Node*> ComponentMapping;

class ComponentTree
{
public:
    ComponentTree(const Image& image);
    ComponentTree(const ComponentTree&) : totalNodes(0), componentMapping(),
        root(0), lcaSolver(0)
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
    bool isLeaf(Node *node) const { return node->getChilds().empty(); }
    Node* getComponent(unsigned int pixelPosition) const
    { return componentMapping.at(pixelPosition); }

private:
    void buildComponentTree(const Image& image);

    unsigned int totalNodes;
    ComponentMapping componentMapping;
    Node* root;
    LCASolver* lcaSolver;
};

#endif // __COMPONENTTREE_H_INCLUDED__
