#ifndef __LCASOLVER_H_INCLUDED__
#define __LCASOLVER_H_INCLUDED__

#include <vector>
#include <unordered_map>
#include <stdexcept>
#include "Node.h"

class LCASolver
{
public:
    LCASolver(Node *root);
    LCASolver(LCASolver&) : root(0), eulerTour(), representatives(), tc(0)
        { throw std::runtime_error("Not implemented yet!"); }
    ~LCASolver() { if(tc != 0) delete[] tc; tc = 0; }
    LCASolver& operator=(const LCASolver&)
    { throw std::runtime_error("Not implemented yet!"); }

    Node* getLCA(Node* a, Node* b);

private:
    void doEulerTour(Node* node);
    unsigned short getLevelRMQ(size_t position1, size_t position2);
    void buildSparceTable();

    Node* root;
    std::vector<Node*> eulerTour;
    std::unordered_map<Node*,size_t> representatives;
    unsigned short* tc;
};

#endif // __LCASOLVER_H_INCLUDED__
