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

#ifndef Swing_Node_H
#define Swing_Node_H

#include "SERenderingEngineLIB.h"
#include "SESpatial.h"
#include "SECuller.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20080723
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SENode : public SESpatial
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    SENode(void);
    virtual ~SENode(void);

    // Child access.
    inline int GetChildCount(void) const;
    int AttachChild(SESpatial* pChild);
    int DetachChild(SESpatial* pChild);
    SESpatialPtr DetachChildAt(int i);
    SESpatialPtr SetChild(int i, SESpatial* pChild);
    SESpatialPtr GetChild(int i);

protected:
    // Support for the geometric update.
    virtual void UpdateWorldData(double dAppTime);
    virtual void UpdateWorldBound(void);

    // Support for hierarchical culling.
    virtual void GetUnculledSet(SECuller& rCuller, bool bNoCull);

    // Child pointers.
    std::vector<SESpatialPtr> m_Child;
};

typedef SESmartPointer<SENode> SENodePtr;

#include "SENode.inl"

}

#endif
