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

#ifndef Swing_ConditionVariableType_H
#define Swing_ConditionVariableType_H

#include "SECoreLIB.h"
#include "SEPlatforms.h"

#if defined(SE_IS_WINDOWS)
//----------------------------------------------------------------------------
namespace Swing
{
    typedef struct
    {
        // Count of the number of waiters.
        SE_UInt32 WaitersCount;

        // Serialize access to <waitersCount>.
        void* pCriticalSectionWaitersCountMutex;
        void* pCriticalSectionConditionMutex;

        // Signal and broadcast event HANDLEs.
        enum
        {
            SIGNAL     = 0,
            BROADCAST  = 1,
            NUM_EVENTS = 2
        };
        void* hEvents[NUM_EVENTS];
    }
    SEConditionVariableType;
}
//----------------------------------------------------------------------------
#else
//----------------------------------------------------------------------------
// TODO:
//----------------------------------------------------------------------------
#error Other platforms not yet implemented.
#endif

#endif
