// Swing Engine Version 2 Source Code 
// Most of techniques in the engine are mainly based on David Eberly's
// Wild Magic 4 and 5 open-source game engine. The author of Swing Engine 
// learned a lot from Eberly's experience of architecture and algorithm.
// Several subsystems are totally new, and others are reorganized or
// reimplimented based on Wild Magic's subsystems.
// Copyright (c) 2007-2011
//
// David Eberly's permission:
// Geometric Tools, LLC
// Copyright (c) 1998-2010
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt

#ifndef Swing_IntArray_H
#define Swing_IntArray_H

#include "SEPlatforms.h"
#include "SESharedArray.h"

namespace Swing
{
//----------------------------------------------------------------------------
// Description:
// Date:20080324
//----------------------------------------------------------------------------
typedef SESharedArray<int> SEIntArray;
typedef SESmartPointer<SEIntArray> SEIntArrayPtr;
}

#endif
