#include <algorithm>

#include "WDestructibleElementsCollection.h"

using namespace std;

void WDestructibleElementsCollection::addElement(const WDestructibleElement& element)
{
    if(mapping.find(element.pixelPosition) != mapping.end())
    {
        if(mapping.at(element.pixelPosition) == element)
        {
            return;
        }
        else
        {
            // update collection
            collection.erase(mapping.at(element.pixelPosition));
            collection.insert(element);

            // update mapping
            mapping.at(element.pixelPosition) = element;
        }
    }
    else
    {
        mapping.insert(pair<size_t,WDestructibleElement>(element.pixelPosition,element));
        collection.insert(element);
    }
}

void WDestructibleElementsCollection::removeElement(const WDestructibleElement& element)
{
    if(mapping.erase(element.pixelPosition) == 1)
    {
        collection.erase(element);
    }
}

void WDestructibleElementsCollection::removeElement(size_t pixelPosition)
{
    if(mapping.find(pixelPosition) != mapping.end())
    {
        collection.erase(mapping.at(pixelPosition));
        mapping.erase(pixelPosition);
    }
}

WDestructibleElement WDestructibleElementsCollection::getMinimum()
{
    WDestructibleElement element = *collection.begin();
    collection.erase(collection.begin());
    mapping.erase(element.pixelPosition);
    return element;
}
