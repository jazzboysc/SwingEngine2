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

#ifndef Swing_RTTIMCR_H
#define Swing_RTTIMCR_H

//----------------------------------------------------------------------------
#define SE_DECLARE_RTTI \
public: \
    static const SERTTI TYPE; \
    virtual const SERTTI& GetType(void) const { return TYPE; }
//----------------------------------------------------------------------------
#define SE_DECLARE_TEMPLATE_RTTI \
public: \
    SE_CORE_API static const SERTTI TYPE; \
    virtual const SERTTI& GetType(void) const { return TYPE; }
//----------------------------------------------------------------------------
#define SE_IMPLEMENT_RTTI(nsname, classname, baseclassname) \
    const SERTTI classname::TYPE(#nsname"."#classname, &baseclassname::TYPE)
//----------------------------------------------------------------------------
#define SE_IMPLEMENT_TEMPLATE_RTTI(nsname, classname, baseclassname) \
    template <> \
    const SERTTI classname::TYPE(#nsname"."#classname, &baseclassname::TYPE)
//----------------------------------------------------------------------------

#endif
