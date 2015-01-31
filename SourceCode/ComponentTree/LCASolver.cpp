#include <limits>
#include <cmath>
#include <algorithm>
#include "LCASolver.h"
#include <cassert>

using namespace std;

void LCASolver::buildSparceTable()
{
    size_t vectorSize = eulerTour.size();
    unsigned int logBaseTwoVectorSize = ceil(log2(vectorSize));

    tc = new unsigned short[vectorSize * logBaseTwoVectorSize];
    for(unsigned int i = 0; i < vectorSize; i++)
    {
        tc[i*logBaseTwoVectorSize] = i;
    }

    for(unsigned int j = 1; j < logBaseTwoVectorSize; j++)
    {
        unsigned int blockSize = pow(2,j-1);
        unsigned int n = vectorSize - (blockSize * 2);
        for(unsigned int i = 0; i < n; i++)
        {
            unsigned int maximumBlockOnePosition = tc[i*logBaseTwoVectorSize + j-1];

            unsigned int maximumBlockTwoPosition = tc[(i+blockSize)*logBaseTwoVectorSize + j-1];

            if(eulerTour.at(maximumBlockOnePosition)->getLevel() >=
                    eulerTour.at(maximumBlockTwoPosition)->getLevel())
            {
                tc[i*logBaseTwoVectorSize + j] = maximumBlockOnePosition;
            }
            else
            {
                tc[i*logBaseTwoVectorSize + j] = maximumBlockTwoPosition;
            }
        }
    }
}

void LCASolver::doEulerTour(Node* node)
{
    representatives[node] = eulerTour.size();
    eulerTour.push_back(node);
    NodeSet childs = node->getChilds();
    for (NodeSet::iterator it = childs.begin(); it != childs.end(); ++it)
    {
        doEulerTour(*it);
        eulerTour.push_back(node);
    }
}

LCASolver::LCASolver(Node* root) : root(root), eulerTour(), representatives(),
    tc(0)
{
    if(!root)
    {
        throw invalid_argument("root node is null");
    }

    doEulerTour(root);
    buildSparceTable();
}

Node* LCASolver::getLCA(Node *a, Node *b)
{
    if(a == b)
    {
        throw invalid_argument("a and b must be different");
    }
    return eulerTour.at(getLevelRMQ(representatives.at(a),representatives.at(b)));
}

unsigned short LCASolver::getLevelRMQ(size_t position1, size_t position2)
{
    // position1 must be the lower value, so swapping if needed
    if(position1 > position2)
    {
        size_t tmp = position1;
        position1 = position2;
        position2 = tmp;
    }

    size_t vectorSize = eulerTour.size();
    unsigned int logBaseTwoVectorSize = ceil(log2(vectorSize));

    // floor of log base 2 (position2 - position1)
    unsigned int logBase2BetweenPosition = log2(position2 - position1);

    // position in euler tour between range[position1,position1 + 2^logBase2BetweenPosition]
    // of the maximum level between
    unsigned int maxOnRange1 = tc[position1*logBaseTwoVectorSize
            + logBase2BetweenPosition];

    // position in euler tour between range[position2 - pow(2,logBase2BetweenPosition) + 1,
    // position2 + 2^logBase2BetweenPosition] of the maximum level between
    unsigned int position = (position2 - pow(2,logBase2BetweenPosition)) *
            logBaseTwoVectorSize + logBase2BetweenPosition;
    unsigned int maxOnRange2 = tc[position];

    if(eulerTour.at(maxOnRange1)->getLevel() > eulerTour.at(maxOnRange2)->getLevel())
    {
        return maxOnRange1;
    }
    else
    {
        return maxOnRange2;
    }
}
