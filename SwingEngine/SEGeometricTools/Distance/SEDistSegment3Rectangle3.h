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

#ifndef Swing_DistSegment3Rectangle3_H
#define Swing_DistSegment3Rectangle3_H

#include "SEGeometricToolsLIB.h"
#include "SEDistance.h"
#include "SESegment3.h"
#include "SERectangle3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20090119
//----------------------------------------------------------------------------
class SE_GEOMETRICTOOLS_API SEDistSegment3Rectangle3f : public SEDistance<float, 
    SEVector3f>
{
public:
    SEDistSegment3Rectangle3f(const SESegment3f& rSegment,
        const SERectangle3f& rRectangle);

    // �������.
    const SESegment3f& GetSegment(void) const;
    const SERectangle3f& GetRectangle(void) const;

    // static distance��ѯ.
    virtual float Get(void);
    virtual float GetSquared(void);

    // ����dynamic distance��ѯ��convex function����.
    virtual float Get(float fT, const SEVector3f& rVelocity0,
        const SEVector3f& rVelocity1);
    virtual float GetSquared(float fT, const SEVector3f& rVelocity0,
        const SEVector3f& rVelocity1);

    // ����������Ϣ.
    float GetSegmentParameter(void) const;
    float GetRectangleCoordinate(int i) const;

private:
    const SESegment3f* m_pSegment;
    const SERectangle3f* m_pRectangle;

    // ����������Ϣ.
    float m_fSegmentParameter;  // closest0 = seg.origin+param*seg.direction
    float m_afRectCoord[2];     // closest1 = rect.center+param0*rect.dir0+
                               //            param1*rect.dir1
};

}

#endif
