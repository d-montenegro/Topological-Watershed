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
    Element* e = new Element(element);
    if(!disjointSet.insert(
                pair<unsigned int,Element*>(element,e)).second)
    {
        delete e;
        e = 0;
        throw invalid_argument("element already exists");
    }
}

unsigned int DisjointSetCollection::find(unsigned int element)
{
    Element* setElement = 0;
    try
    {
        setElement = disjointSet.at(element);
    }
    catch(out_of_range e)
    {
        throw invalid_argument("element does not exists");
    }

    if (setElement->father != element)
    {
        setElement->father = find(setElement->father);
    }
    return setElement->father;
}

unsigned int DisjointSetCollection::linkSets(unsigned int element1, unsigned int element2)
{
    const unsigned int canonicalElement1 = find(element1);
    const unsigned int canonicalElement2 = find(element2);

    if (canonicalElement1 == canonicalElement2)
    {
        throw invalid_argument("elements already belongs to the same set");
    }

    Element* setCanonicalElement1 = disjointSet.at(canonicalElement1);
    Element* setCanonicalElement2 = disjointSet.at(canonicalElement2);

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
