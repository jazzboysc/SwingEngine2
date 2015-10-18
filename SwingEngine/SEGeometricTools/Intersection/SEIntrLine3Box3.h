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

#ifndef Swing_IntrLine3Box3_H
#define Swing_IntrLine3Box3_H

#include "SEGeometricToolsLIB.h"
#include "SEIntersector.h"
#include "SELine3.h"
#include "SEBox3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081230
//----------------------------------------------------------------------------
class SE_GEOMETRICTOOLS_API SEIntrLine3Box3f : public SEIntersector<float, 
    SEVector3f>
{
public:
    SEIntrLine3Box3f(const SELine3f& rLine, const SEBox3f& rBox);

    // �������.
    const SELine3f& GetLine(void) const;
    const SEBox3f& GetBox(void) const;

    // static intersection��ѯ.
    virtual bool Test(void); // ʹ��Ǳ�ڷ������Minkowski difference�㷨
    virtual bool Find(void);

    // �ཻ����.
    int GetCount(void) const;
    const SEVector3f& GetPoint(int i) const;

private:
    static bool Clip(float fDenom, float fNumer, float& rfT0, float& rfT1);

    // ������Ƿ��ཻ�Ķ���.
    const SELine3f* m_pLine;
    const SEBox3f* m_pBox;

    // �ཻ�������Ϣ.
    int m_iCount;
    SEVector3f m_aPoint[2];

// Internal use(�����SEIntrRay3Box3f��SEIntrSegment3Box3f)
public:
    // ʹ�����Ѷ�-Barsky�ü��㷨,����linear component��box�Ľ���.
    static bool DoClipping(float fT0, float fT1, const SEVector3f& rOrigin,
        const SEVector3f& rDirection, const SEBox3f& rBox, bool bSolid, 
        int& riCount, SEVector3f aPoint[2], int& riIntrType);
};

}

#endif
