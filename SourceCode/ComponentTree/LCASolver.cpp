#include <iostream>
#include <limits>
#include <cmath>
#include <algorithm>
#include "LCASolver.h"

using namespace std;

void LCASolver::buildSparceTable()
{
    double logBaseTwoVectorSize = log2(levelVector.size());
    for(unsigned int i = 0; i < levelVector.size(); i++)
    {
        for(unsigned int j = 0; j < logBaseTwoVectorSize; j++)
        {
            unsigned short minimum = numeric_limits<ushort>::max();
            unsigned int position = numeric_limits<unsigned int>::max();
            for(unsigned int k = i; k <= i + pow(2,j) - 1; k++)
            {
                try
                {
                    if(levelVector.at(k) < minimum)
                    {
                        minimum = levelVector.at(k);
                        position = k;
                    }
                }
                catch(out_of_range& e)
                {
                    break;
                }
            }
            cout << "Sparce Table (" << i << "," << j << "): " << position << endl;
            tc[pair<unsigned int,unsigned int>(i,j)] = position;
        }
    }
}


void LCASolver::calculateRepresentatives(Node* node)
{
    unsigned int pos = 0;
    for (vector<Node*>::iterator it = eulerTour.begin(); it != eulerTour.end(); ++it)
    {
        if ((*it) == node)
        {
            representatives[node] = pos;
            break;
        }
        pos++;
    }
    NodeSet childs = node->getChilds();
    for (NodeSet::iterator it = childs.begin(); it != childs.end(); ++it)
    {
        calculateRepresentatives(*it);
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

LCASolver::LCASolver(Node* root) : root(root), eulerTour(), levelVector(),
    representatives(), tc()
{
    if(!root)
    {
        throw invalid_argument("root node is null");
    }

    doEulerTour(root);
    for_each(eulerTour.begin(),eulerTour.end(),[&](Node* n)
    {levelVector.push_back(n->getLevel());});
    calculateRepresentatives(root);
    buildSparceTable();
}

Node* LCASolver::getLCA(Node *a, Node *b)
{
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
    // of the minimun level between
    unsigned int minOnRange1 = tc[pair<unsigned int,unsigned int>(position1,
                              logBase2BetweenPosition)];

    // position in euler tour between range[position2 - pow(2,logBase2BetweenPosition) + 1,
    // position2 + 2^logBase2BetweenPosition] of the minimun level between
    unsigned int minOnRange2 = tc[pair<unsigned int,unsigned int>(position2 -
                              pow(2,logBase2BetweenPosition),
                              logBase2BetweenPosition)];

    if(levelVector.at(minOnRange1) < levelVector.at(minOnRange2))
    {
        return minOnRange1;
    }
    else
    {
        return minOnRange2;
    }
}
