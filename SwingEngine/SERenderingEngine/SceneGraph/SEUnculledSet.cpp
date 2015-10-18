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

#include "SERenderingEnginePCH.h"
#include "SEUnculledSet.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEUnculledSet::SEUnculledSet(int iMaxCount, int iGrowBy)
{
    m_pUnculled = 0;
    Resize(iMaxCount, iGrowBy);
}
//----------------------------------------------------------------------------
SEUnculledSet::~SEUnculledSet()
{
    SE_DELETE[] m_pUnculled;
}
//----------------------------------------------------------------------------
void SEUnculledSet::Insert(SESpatial* pObject)
{
    if( ++m_iCount > m_iMaxCount )
    {
        int iNewMaxCount = m_iMaxCount + m_iGrowBy;
        SEUnculledObject* pNewVisible = SE_NEW SEUnculledObject[iNewMaxCount];
        size_t uiSize = m_iCount * sizeof(SEUnculledObject);
        SESystem::Memcpy(pNewVisible, uiSize, m_pUnculled, uiSize);
        SE_DELETE[] m_pUnculled;
        m_pUnculled = pNewVisible;
        m_iMaxCount = iNewMaxCount;
    }

    int iIndex = m_iCount-1;
    m_pUnculled[iIndex].Object = pObject;
}
//----------------------------------------------------------------------------
void SEUnculledSet::Resize(int iMaxCount, int iGrowBy)
{
    if( iMaxCount > 0 )
    {
        m_iMaxCount = iMaxCount;
    }
    else
    {
        m_iMaxCount = US_DEFAULT_MAX_COUNT;
    }

    if( iGrowBy > 0 )
    {
        m_iGrowBy = iGrowBy;
    }
    else
    {
        m_iGrowBy = US_DEFAULT_GROWBY;
    }

    SE_DELETE[] m_pUnculled;
    m_iCount = 0;
    m_pUnculled = SE_NEW SEUnculledObject[m_iMaxCount];
}
//----------------------------------------------------------------------------
