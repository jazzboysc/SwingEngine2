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

#ifndef Swing_Assert_H
#define Swing_Assert_H

#ifdef NDEBUG

//----------------------------------------------------------------------------
#define SE_ASSERT(ignore) \
((void)0)
//----------------------------------------------------------------------------

#else

//----------------------------------------------------------------------------
// Microsoft Windows 2000/XP platform
//----------------------------------------------------------------------------
#if defined(_WIN32)

//----------------------------------------------------------------------------
#define SE_SAFEBREAKPOINT \
try \
{ \
    _asm \
    { \
        int 3 \
    }; \
} \
catch(...) \
{ \
    ; \
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Macintosh OS X platform
//----------------------------------------------------------------------------
#elif defined(__APPLE__)

#define SE_SAFEBREAKPOINT ((void)0)

//----------------------------------------------------------------------------
// Linux platform
//----------------------------------------------------------------------------
#else

#define SE_SAFEBREAKPOINT ((void)0)

#endif

//----------------------------------------------------------------------------
#define SE_ASSERT(expr) \
if( !(expr) ) \
{ \
    FILE* assertFile = SESystem::Fopen("Assert.txt", "at"); \
    SESystem::Fprintf(assertFile, "SwingEngine assert:\n"); \
    SESystem::Fprintf(assertFile, "File: %s\n", __FILE__); \
    SESystem::Fprintf(assertFile, "Line: %d\n", __LINE__); \
    SESystem::Fprintf(assertFile, "Function: %s\n", __FUNCTION__); \
    SESystem::Fclose(assertFile); \
    SE_SAFEBREAKPOINT; \
    assert(expr); \
}
//----------------------------------------------------------------------------

#endif

#endif

