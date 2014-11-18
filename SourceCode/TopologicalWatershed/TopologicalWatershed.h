#ifndef __TOPOLOGICALWATERSHED_H_INCLUDED__
#define __TOPOLOGICALWATERSHED_H_INCLUDED__

#include "Image.h"
#include "ComponentTree.h"

void doLinearTopologicalWatershed(Image& image, ComponentTree& componentTree);

void doParallelTopologicalWatershed(Image& image, ComponentTree& componentTree,
                                    ushort numberOfThreads);

#endif // __TOPOLOGICALWATERSHED_H_INCLUDED__
