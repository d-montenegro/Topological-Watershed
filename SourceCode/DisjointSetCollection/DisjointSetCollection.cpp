#include <iostream>
#include <iterator>
#include <stdexcept>

#include "DisjointSetCollection.h"

struct Element
{
    unsigned int father;
    unsigned int ranking;
    Element(const unsigned int element) : father(element),
        ranking(0) {}
    Element() : father(0),ranking(0) {}
};

using namespace std;

DisjointSetCollection::~DisjointSetCollection()
{
    for (auto& element : disjointSet)
    {
        delete element.second;
    }
}

void DisjointSetCollection::addNewSet(unsigned int element)
{
    if (disjointSet.count(element) > 0)
    {
        throw invalid_argument("element already exists");
    }

    disjointSet[element] = new Element(element);
}

unsigned int DisjointSetCollection::find(unsigned int element)
{
    if (0 == disjointSet.count(element))
    {
        throw invalid_argument("element does not exists");
    }

    Element* setElement = disjointSet[element];
    if (setElement->father != element)
    {
        setElement->father = find(setElement->father);
    }
    return setElement->father;
}

unsigned int DisjointSetCollection::linkSets(unsigned int element1, unsigned int element2)
{
    if (0 == disjointSet.count(element1))
    {
        throw invalid_argument("element does not exists");
    }

    if (0 == disjointSet.count(element2))
    {
        throw invalid_argument("element does not exists");
    }

    const unsigned int canonicalElement1 = find(element1);
    const unsigned int canonicalElement2 = find(element2);

    if (canonicalElement1 == canonicalElement2)
    {
        throw invalid_argument("elements already belongs to the same set");
    }

    Element* setCanonicalElement1 = disjointSet[canonicalElement1];
    Element* setCanonicalElement2 = disjointSet[canonicalElement2];

    if (setCanonicalElement1->ranking > setCanonicalElement2->ranking)
    {
        setCanonicalElement2->father = canonicalElement1;
        return canonicalElement1;
    }

    if (setCanonicalElement1->ranking < setCanonicalElement2->ranking)
    {
        setCanonicalElement1->father = canonicalElement2;
        return canonicalElement2;
    }

    // both rankings are equal, let's choose element2 as canonical element
    setCanonicalElement2->ranking++;
    setCanonicalElement1->father = canonicalElement2;
    return canonicalElement2;
}
