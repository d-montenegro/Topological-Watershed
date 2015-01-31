#ifndef __WDESTRUCTIBLEELEMENT_H_INCLUDED__
#define __WDESTRUCTIBLEELEMENT_H_INCLUDED__

#include <iostream>
#include "Node.h"

struct WDestructibleElement
{
    unsigned int pixelPosition;
    unsigned short pixelValue;
    unsigned short priority;
    Node* futureNode;
    bool isMinimal;

    WDestructibleElement(unsigned int pixelPosition, unsigned short pixelValue, unsigned short priority, Node* node, bool isMinimal) :
        pixelPosition(pixelPosition), pixelValue(pixelValue),priority(priority), futureNode(node), isMinimal(isMinimal) {}

    bool operator== (const WDestructibleElement& e) const;

    bool operator< (const WDestructibleElement& e) const;

    WDestructibleElement& operator= (const WDestructibleElement& e);

    void printMe() const;
};


inline bool WDestructibleElement::operator< (const WDestructibleElement& e) const
{
    if(isMinimal && !e.isMinimal)
    {
        return true;
    }
    if(!isMinimal && e.isMinimal)
    {
        return false;
    }
    if(isMinimal && e.isMinimal)
    {
        if(pixelValue == e.pixelValue)
        {
            return priority < e.priority;
        }
        else
        {
            return pixelValue > e.pixelValue;
        }
    }

    return futureNode->getLevel() < e.futureNode->getLevel();
}

inline bool WDestructibleElement::operator== (const WDestructibleElement& e) const
{
    return pixelPosition == e.pixelPosition &&
           pixelValue == e.pixelValue &&
           priority == e.priority &&
           futureNode == e.futureNode &&
           isMinimal == e.isMinimal;
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
