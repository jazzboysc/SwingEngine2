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

#ifndef Swing_IntrSegment3Box3_H
#define Swing_IntrSegment3Box3_H

#include "SEGeometricToolsLIB.h"
#include "SEIntersector.h"
#include "SESegment3.h"
#include "SEBox3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20090203
//----------------------------------------------------------------------------
class SE_GEOMETRICTOOLS_API SEIntrSegment3Box3f : public SEIntersector<float, 
    SEVector3f>
{
public:
    SEIntrSegment3Box3f(const SESegment3f& rSegment, const SEBox3f& rBox, 
        bool bSolid);

    // �������.
    const SESegment3f& GetSegment(void) const;
    const SEBox3f& GetBox(void) const;

    // static intersection��ѯ.
    virtual bool Test(void);
    virtual bool Find(void);

    // �ཻ����.
    int GetCount(void) const;
    const SEVector3f& GetPoint(int i) const;

private:
    // ��������.
    const SESegment3f* m_pSegment;
    const SEBox3f* m_pBox;
    bool m_bSolid;

    // �ཻ�������Ϣ.
    int m_iCount;
    SEVector3f m_aPoint[2];
};

}

#endif
