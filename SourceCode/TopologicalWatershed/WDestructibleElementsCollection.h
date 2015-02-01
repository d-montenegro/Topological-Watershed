#ifndef __WDESTRUCTIBLEELEMENTSCOLLECTION_H_INCLUDED__
#define __WDESTRUCTIBLEELEMENTSCOLLECTION_H_INCLUDED__

#include "WDestructibleElement.h"
#include <unordered_map>
#include <set>

class WDestructibleElementsCollection
{
public:
    WDestructibleElementsCollection(unsigned int maxSize) : collection(),
        mapping() {}
    ~WDestructibleElementsCollection() {}

    /*
     * Adds new element to the collection. Updates if already exists.
     */
    void addElement(const WDestructibleElement& element);

    /*
     * Remove element if exists.
     */
    void removeElement(const WDestructibleElement& element);

    /*
     * Remove element by pixel position if exists.
     */
    void removeElement(size_t pixelPosition);

    /*
     * Returns the minimum WDestructibleElement of the collection.
     * The element is removed from the collection
     */
    WDestructibleElement getMinimum();

    /*
     * checks if the collection is empty
     */
    bool isEmpty() { return collection.empty(); }

    bool isPresent(size_t pixelPosition) { return mapping.find(pixelPosition) != mapping.end(); }

private:
    std::set<WDestructibleElement> collection;
    std::unordered_map<size_t,WDestructibleElement> mapping;
};

#endif // __WDESTRUCTIBLEELEMENTSCOLLECTION_H_INCLUDED__
