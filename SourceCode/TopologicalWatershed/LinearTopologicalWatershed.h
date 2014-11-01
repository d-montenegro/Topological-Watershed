#ifndef __LINEALTOPOLOGICALWATERSHED_H_INCLUDED__
#define __LINEALTOPOLOGICALWATERSHED_H_INCLUDED__

#include "Image.h"
#include "ComponentTree.h"

void doLinearTopologicalWatershed(Image& image, ComponentTree& componentTree);

void doParallelTopologicalWatershed(Image& image, ComponentTree& componentTree,
                                    ushort numberOfThreads);

#endif // __LINEALTOPOLOGICALWATERSHED_H_INCLUDED__
