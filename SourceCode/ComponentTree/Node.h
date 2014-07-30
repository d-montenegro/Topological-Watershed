#ifndef __NODE_H_INCLUDED__
#define __NODE_H_INCLUDED__

#include <vector>

using namespace std;

class Node;
typedef vector<Node*> NodeVector;

class Node
{
public:
    Node(unsigned int l) : level(l), childs() {}
    void addChilds(NodeVector nodes) { childs.insert(childs.end(), nodes.begin(), nodes.end()); }

    unsigned int getLevel() { return level; }
    NodeVector getChilds() { return childs; }

private:
    unsigned int level;
    NodeVector childs;
};

#endif // __NODE_H_INCLUDED__
