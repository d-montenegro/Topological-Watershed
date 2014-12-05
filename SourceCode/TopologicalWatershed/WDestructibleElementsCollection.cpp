#include <algorithm>

#include "WDestructibleElementsCollection.h"

using namespace std;

void WDestructibleElementsCollection::addElement(const WDestructibleElement& element)
{
    removeElement(element.pixelPosition);
    collection.insert(element);
    elementPresent.at(element.pixelPosition) = true;
}

void WDestructibleElementsCollection::removeElement(const WDestructibleElement& element)
{
    if(elementPresent.at(element.pixelPosition))
    {
        collection.erase(element);
        elementPresent.at(element.pixelPosition) = false;
    }
}

void WDestructibleElementsCollection::removeElement(unsigned int pixelPosition)
{
    if(elementPresent.at(pixelPosition))
    {
        set<WDestructibleElement>::iterator it =
                find_if(collection.begin(),collection.end(),
                [&](const WDestructibleElement& e)
                {
                    return e.pixelPosition == pixelPosition;
                });
        collection.erase(it);
        elementPresent.at(pixelPosition) = false;
    }
}

WDestructibleElement WDestructibleElementsCollection::getMinimum()
{
    WDestructibleElement element = *collection.begin();
    collection.erase(collection.begin());
    elementPresent.at(element.pixelPosition) = false;
    return element;
}
