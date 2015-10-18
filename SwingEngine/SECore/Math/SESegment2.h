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

#ifndef Swing_Segment2_H
#define Swing_Segment2_H

#include "SECoreLIB.h"
#include "SEVector2.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20090113
//----------------------------------------------------------------------------
class SE_CORE_API SESegment2f
{
public:
    // �߶α�ʾΪ����������ʽ:P + t*D,����P���߶�ԭ��,D�ǵ�λ���ȷ�������,
    // |t| <= e,����e���߶���չ����.
    // �û�����ȷ��DΪ��λ����.
    // �߶ζ˵���P - e*D��P + e*D.
    // �߶εı�ʾ����������OBB.P���е�,D�ǵ�λ������,e��������չ����.

    SESegment2f(void);
    SESegment2f(const SEVector2f& rOrigin, const SEVector2f& rDirection,
        float fExtent);

    // �˵����.
    SEVector2f GetPosEnd(void) const;  // P + e*D
    SEVector2f GetNegEnd(void) const;  // P - e*D

    SEVector2f Origin, Direction;
    float Extent;
};

}

#endif