#ifndef __WDESTRUCTIBLEELEMENT_H_INCLUDED__
#define __WDESTRUCTIBLEELEMENT_H_INCLUDED__

#include <iostream>
#include "Node.h"

struct WDestructibleElement
{
    unsigned int pixelPosition;
    mutable Node* futureNode;

    WDestructibleElement(unsigned int pixelPosition, Node* node) :
        pixelPosition(pixelPosition), futureNode(node) {}

    bool operator== (const WDestructibleElement& e) const;

    bool operator< (const WDestructibleElement& e) const;

    WDestructibleElement& operator= (const WDestructibleElement& e);

    void printMe() const;
};


inline bool WDestructibleElement::operator< (const WDestructibleElement& e) const
{
    if (futureNode->getLevel() == e.futureNode->getLevel())
    {
        return pixelPosition < e.pixelPosition;
    }
    else
    {
        return futureNode->getLevel() < e.futureNode->getLevel();
    }
}

inline bool WDestructibleElement::operator== (const WDestructibleElement& e) const
{
    return pixelPosition == e.pixelPosition;
}

inline WDestructibleElement& WDestructibleElement::operator=
    (const WDestructibleElement& e)
{
    pixelPosition = e.pixelPosition;
    futureNode = e.futureNode;
    return *this;
}

inline void WDestructibleElement::printMe() const
{
    std::cout << "Pixel position: " << pixelPosition << " - Node level: "
         << futureNode->getLevel() << std::endl;
}

#endif // __WDESTRUCTIBLEELEMENT_H_INCLUDED__
