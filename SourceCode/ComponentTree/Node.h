#ifndef __NODE_H_INCLUDED__
#define __NODE_H_INCLUDED__

#include <set>

using namespace std;

class Node;
using NodeSet = set<Node*>;

class Node
{
public:
    Node(unsigned short l) : level(l), childs() {}
    void addChilds(NodeSet nodes) { childs.insert(nodes.begin(),nodes.end()); }
    void addChild(Node* node) { childs.insert(node); }

    unsigned short getLevel() { return level; }
    NodeSet getChilds() { return childs; }

private:
    const unsigned short level;
    NodeSet childs;
};

#endif // __NODE_H_INCLUDED__
