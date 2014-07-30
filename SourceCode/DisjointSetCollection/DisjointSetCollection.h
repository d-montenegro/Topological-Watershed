#ifndef __DISJOINTSETCOLLECTION_H_INCLUDED__
#define __DISJOINTSETCOLLECTION_H_INCLUDED__

#include <set>
#include <map>

using namespace std;

struct Element;

class DisjointSetCollection
{
public:
    ~DisjointSetCollection();
    /*
     * Adds the set {element} to the collection, 'element' must not belongs to
     * another set in the collection.
     */
    void addNewSet(unsigned int element);

    /*
     * Returns the canonical element of the set that 'element' belongs to.
     */
    unsigned int find(unsigned int element);

    /*
     * Links the set 'element1' belongs to with the set 'element2' belongs to.
     * Both elements must not belongs to the same set.
     */
    unsigned int linkSets(unsigned int element1, unsigned int element2);

private:
    map<unsigned int, Element*> disjointSet;
};

#endif // __DISJOINTSETCOLLECTION_H_INCLUDED__
