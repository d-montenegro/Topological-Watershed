#ifndef __WDESTRUCTIBLEELEMENTSCOLLECTION_H_INCLUDED__
#define __WDESTRUCTIBLEELEMENTSCOLLECTION_H_INCLUDED__

#include "WDestructibleElement.h"
#include <vector>
#include <set>

class WDestructibleElementsCollection
{
public:
    WDestructibleElementsCollection(unsigned int maxSize) : collection(),
        elementPresent(maxSize,false) {}
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
    void removeElement(unsigned int pixelPosition);

    /*
     * Returns the minimum WDestructibleElement of the collection.
     * The element is removed from the collection
     */
    WDestructibleElement getMinimum();

    /*
     * checks if the collection is empty
     */
    bool isEmpty() { return collection.empty(); }

    bool isPresent(unsigned int pixelPosition) { return elementPresent.at(pixelPosition); }

private:
    std::set<WDestructibleElement> collection;
    std::vector<bool> elementPresent;
};

#endif // __WDESTRUCTIBLEELEMENTSCOLLECTION_H_INCLUDED__
