#ifndef __NODE_H_INCLUDED__
#define __NODE_H_INCLUDED__

#include <vector>

using namespace std;

class Node;
using NodeVector = vector<Node*>;

class Node
{
public:
    Node(unsigned short l) : level(l), childs() {}
    void addChilds(NodeVector nodes) { childs.insert(childs.end(), nodes.begin(), nodes.end()); }

    unsigned short getLevel() { return level; }
    NodeVector getChilds() { return childs; }

private:
    unsigned short level;
    NodeVector childs;
};

#endif // __NODE_H_INCLUDED__
