#include <limits>
#include <cmath>
#include <algorithm>
#include "LCASolver.h"

using namespace std;

void LCASolver::buildSparceTable()
{
    size_t vectorSize = eulerTour.size();
    for(unsigned int i = 0; i < vectorSize; i++)
    {
        tc[pair<unsigned int,unsigned int>(i,0)] = i;
    }

    double logBaseTwoVectorSize = log2(vectorSize);
    for(unsigned int j = 1; j < logBaseTwoVectorSize; j++)
    {
        unsigned int blockSize = pow(2,j-1);
        unsigned int n = vectorSize - (blockSize * 2);
        for(unsigned int i = 0; i < n; i++)
        {
            unsigned int maximumBlockOnePosition =
                    tc[pair<unsigned int,unsigned int>(i,j-1)];
            unsigned int maximumBlockTwoPosition =
                    tc[pair<unsigned int,unsigned int>(i+blockSize,j-1)];

            if(eulerTour.at(maximumBlockOnePosition)->getLevel() >=
                    eulerTour.at(maximumBlockTwoPosition)->getLevel())
            {
                tc[pair<unsigned int,unsigned int>(i,j)] = maximumBlockOnePosition;
            }
            else
            {
                tc[pair<unsigned int,unsigned int>(i,j)] = maximumBlockTwoPosition;
            }
        }
    }
}


void LCASolver::calculateRepresentatives()
{
    unsigned int pos = 0;
    for (vector<Node*>::iterator it = eulerTour.begin(); it != eulerTour.end(); ++it)
    {
        if (representatives.find(*it) == representatives.end())
        {
            representatives[*it] = pos;
        }
        pos++;
    }
}

void LCASolver::doEulerTour(Node* node)
{
    eulerTour.push_back(node);
    NodeSet childs = node->getChilds();
    for (NodeSet::iterator it = childs.begin(); it != childs.end(); ++it)
    {
        doEulerTour(*it);
        eulerTour.push_back(node);
    }
}

LCASolver::LCASolver(Node* root) : root(root), eulerTour(), representatives(),
    tc()
{
    if(!root)
    {
        throw invalid_argument("root node is null");
    }

    doEulerTour(root);
    calculateRepresentatives();
    buildSparceTable();
}

Node* LCASolver::getLCA(Node *a, Node *b)
{
    if(a == b)
    {
        throw invalid_argument("a are b must be different");
    }
    return eulerTour.at(getLevelRMQ(representatives.at(a),representatives.at(b)));
}

unsigned short LCASolver::getLevelRMQ(unsigned int position1, unsigned int position2)
{
    // position1 must be the lower value, so swapping if needed
    if(position1 > position2)
    {
        unsigned short tmp = position1;
        position1 = position2;
        position2 = tmp;
    }

    // floor of log base 2 (position2 - position1)
    unsigned int logBase2BetweenPosition = log2(position2 - position1);

    // position in euler tour between range[position1,position1 + 2^logBase2BetweenPosition]
    // of the maximum level between
    unsigned int maxOnRange1 = tc[pair<unsigned int,unsigned int>(position1,
                              logBase2BetweenPosition)];

    // position in euler tour between range[position2 - pow(2,logBase2BetweenPosition) + 1,
    // position2 + 2^logBase2BetweenPosition] of the maximum level between
    unsigned int maxOnRange2 = tc[pair<unsigned int,unsigned int>(position2 -
                              pow(2,logBase2BetweenPosition),
                              logBase2BetweenPosition)];

    if(eulerTour.at(maxOnRange1)->getLevel() > eulerTour.at(maxOnRange2)->getLevel())
    {
        return maxOnRange1;
    }
    else
    {
        return maxOnRange2;
    }
}
