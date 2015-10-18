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

#ifndef Swing_RWMutexType_H
#define Swing_RWMutexType_H

#include "SECoreLIB.h"
#include "SEPlatforms.h"

#if defined(_WIN32)
//----------------------------------------------------------------------------
namespace Swing
{
    typedef struct
    {
        SE_Int32 WritersWaitingCount;
        SE_Int32 ReadersWaitingCount;
        
        // HIWORD is writer active flag;
        // LOWORD is readers active count;
        SE_UInt32 ActiveWriterReaders;
        
        void* hReadyToRead;
        void* hReadyToWrite;
        void* pCriticalSection;
    }
    SERWMutexType;
}
//----------------------------------------------------------------------------
#elif defined(__LINUX__) || defined(__APPLE__)
//----------------------------------------------------------------------------
// TODO:
//----------------------------------------------------------------------------
#error Other platforms not yet implemented.
#else
//----------------------------------------------------------------------------
// TODO: Mutex types for other platforms.
//----------------------------------------------------------------------------
#error Other platforms not yet implemented.
#endif

#endif
