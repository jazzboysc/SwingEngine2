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

#include "SECorePCH.h"
#include "SEFloat4.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEFloat4::SEFloat4()
{
}
//----------------------------------------------------------------------------
SEFloat4::~SEFloat4()
{
}
//----------------------------------------------------------------------------
SEFloat4::SEFloat4(float f0, float f1, float f2, float f3)
{
    m_Tuple[0] = f0;
    m_Tuple[1] = f1;
    m_Tuple[2] = f2;
    m_Tuple[3] = f3;
}
//----------------------------------------------------------------------------
SEFloat4::SEFloat4(const SEFloat4& rTuple)
{
    m_Tuple[0] = rTuple.m_Tuple[0];
    m_Tuple[1] = rTuple.m_Tuple[1];
    m_Tuple[2] = rTuple.m_Tuple[2];
    m_Tuple[3] = rTuple.m_Tuple[3];
}
//----------------------------------------------------------------------------
SEFloat4& SEFloat4::operator=(const SEFloat4& rTuple)
{
    m_Tuple[0] = rTuple.m_Tuple[0];
    m_Tuple[1] = rTuple.m_Tuple[1];
    m_Tuple[2] = rTuple.m_Tuple[2];
    m_Tuple[3] = rTuple.m_Tuple[3];
    return *this;
}
//----------------------------------------------------------------------------
