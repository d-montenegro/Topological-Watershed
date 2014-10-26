#ifndef __LINEALTOPOLOGICALWATERSHED_H_INCLUDED__
#define __LINEALTOPOLOGICALWATERSHED_H_INCLUDED__

#include "Image.h"
#include "ComponentTree.h"

/*
 * Checks if the pixel at pixelPosition is wDestructible or not. If it is,
 * the a pointer to the node this pixel should point to is returned, otherwise,
 * 0 es returned.
 */
Node* wDestructible(const Image& image, ComponentTree& componentTree,
                    const unsigned int pixelPosition);

void doLinearTopologicalWatershed(Image& image, ComponentTree& componentTree);

#endif // __LINEALTOPOLOGICALWATERSHED_H_INCLUDED__


