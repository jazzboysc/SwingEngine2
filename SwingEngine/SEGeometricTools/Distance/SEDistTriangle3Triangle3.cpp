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

#include "SEGeometricToolsPCH.h"
#include "SEDistTriangle3Triangle3.h"
#include "SEDistSegment3Triangle3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDistTriangle3Triangle3f::SEDistTriangle3Triangle3f(const SETriangle3f& 
    rTriangle0, const SETriangle3f& rTriangle1)
    :
    m_pTriangle0(&rTriangle0),
    m_pTriangle1(&rTriangle1)
{
}
//----------------------------------------------------------------------------
const SETriangle3f& SEDistTriangle3Triangle3f::GetTriangle0() const
{
    return *m_pTriangle0;
}
//----------------------------------------------------------------------------
const SETriangle3f& SEDistTriangle3Triangle3f::GetTriangle1() const
{
    return *m_pTriangle1;
}
//----------------------------------------------------------------------------
float SEDistTriangle3Triangle3f::Get()
{
    float fSqrDist = GetSquared();

    return SEMath<float>::Sqrt(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistTriangle3Triangle3f::GetSquared()
{
    // 把triangle0的各边和triangle1进行比较.
    float fSqrDist = SEMath<float>::MAX_REAL, fSqrDistTmp;
    SESegment3f tempSeg;
    float fRatio;
    int i0, i1;
    for( i0 = 2, i1 = 0; i1 < 3; i0 = i1++ )
    {
        tempSeg.Origin = 0.5f*(m_pTriangle0->V[i0] + m_pTriangle0->V[i1]);
        tempSeg.Direction = m_pTriangle0->V[i1] - m_pTriangle0->V[i0];
        tempSeg.Extent = 0.5f*tempSeg.Direction.Normalize();
        SEDistSegment3Triangle3f tempSTDist(tempSeg, *m_pTriangle1);
        fSqrDistTmp = tempSTDist.GetSquared();
        if( fSqrDistTmp < fSqrDist )
        {
            m_ClosestPoint0 = tempSTDist.GetClosestPoint0();
            m_ClosestPoint1 = tempSTDist.GetClosestPoint1();
            fSqrDist = fSqrDistTmp;

            fRatio = tempSTDist.GetSegmentParameter()/tempSeg.Extent;
            m_afTriangleBary0[i0] = 0.5f*(1.0f - fRatio);
            m_afTriangleBary0[i1] = 1.0f - m_afTriangleBary0[i0];
            m_afTriangleBary0[3 - i0 - i1] = 0.0f;
            m_afTriangleBary1[0] = tempSTDist.GetTriangleBary(0);
            m_afTriangleBary1[1] = tempSTDist.GetTriangleBary(1);
            m_afTriangleBary1[2] = tempSTDist.GetTriangleBary(2);

            if( fSqrDist <= SEMath<float>::ZERO_TOLERANCE )
            {
                return 0.0f;
            }
        }
    }

    // 把triangle1的各边和triangle0进行比较.
    for( i0 = 2, i1 = 0; i1 < 3; i0 = i1++ )
    {
        tempSeg.Origin = 0.5f*(m_pTriangle1->V[i0] + m_pTriangle1->V[i1]);
        tempSeg.Direction = m_pTriangle1->V[i1] - m_pTriangle1->V[i0];
        tempSeg.Extent = 0.5f*tempSeg.Direction.Normalize();
        SEDistSegment3Triangle3f tempSTDist(tempSeg, *m_pTriangle0);
        fSqrDistTmp = tempSTDist.GetSquared();
        if( fSqrDistTmp < fSqrDist )
        {
            m_ClosestPoint0 = tempSTDist.GetClosestPoint0();
            m_ClosestPoint1 = tempSTDist.GetClosestPoint1();
            fSqrDist = fSqrDistTmp;

            fRatio = tempSTDist.GetSegmentParameter()/tempSeg.Extent;
            m_afTriangleBary1[i0] = 0.5f*(1.0f - fRatio);
            m_afTriangleBary1[i1] = 1.0f - m_afTriangleBary1[i0];
            m_afTriangleBary1[3 - i0 - i1] = 0.0f;
            m_afTriangleBary0[0] = tempSTDist.GetTriangleBary(0);
            m_afTriangleBary0[1] = tempSTDist.GetTriangleBary(1);
            m_afTriangleBary0[2] = tempSTDist.GetTriangleBary(2);

            if( fSqrDist <= SEMath<float>::ZERO_TOLERANCE )
            {
                return 0.0f;
            }
        }
    }

    return fSqrDist;
}
//----------------------------------------------------------------------------
float SEDistTriangle3Triangle3f::Get(float fT, const SEVector3f& rVelocity0, 
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMV00 = m_pTriangle0->V[0] + fT*rVelocity0;
    SEVector3f vec3fMV01 = m_pTriangle0->V[1] + fT*rVelocity0;
    SEVector3f vec3fMV02 = m_pTriangle0->V[2] + fT*rVelocity0;
    SEVector3f vec3fMV10 = m_pTriangle1->V[0] + fT*rVelocity1;
    SEVector3f vec3fMV11 = m_pTriangle1->V[1] + fT*rVelocity1;
    SEVector3f vec3fMV12 = m_pTriangle1->V[2] + fT*rVelocity1;
    SETriangle3f tempMTriangle0(vec3fMV00, vec3fMV01, vec3fMV02);
    SETriangle3f tempMTriangle1(vec3fMV10, vec3fMV11, vec3fMV12);

    return SEDistTriangle3Triangle3f(tempMTriangle0, tempMTriangle1).Get();
}
//----------------------------------------------------------------------------
float SEDistTriangle3Triangle3f::GetSquared(float fT, 
    const SEVector3f& rVelocity0, const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMV00 = m_pTriangle0->V[0] + fT*rVelocity0;
    SEVector3f vec3fMV01 = m_pTriangle0->V[1] + fT*rVelocity0;
    SEVector3f vec3fMV02 = m_pTriangle0->V[2] + fT*rVelocity0;
    SEVector3f vec3fMV10 = m_pTriangle1->V[0] + fT*rVelocity1;
    SEVector3f vec3fMV11 = m_pTriangle1->V[1] + fT*rVelocity1;
    SEVector3f vec3fMV12 = m_pTriangle1->V[2] + fT*rVelocity1;
    SETriangle3f tempMTriangle0(vec3fMV00, vec3fMV01, vec3fMV02);
    SETriangle3f tempMTriangle1(vec3fMV10, vec3fMV11, vec3fMV12);

    return SEDistTriangle3Triangle3f(tempMTriangle0, 
        tempMTriangle1).GetSquared();
}
//----------------------------------------------------------------------------
float SEDistTriangle3Triangle3f::GetTriangleBary0(int i) const
{
    SE_ASSERT( 0 <= i && i < 3 );

    return m_afTriangleBary0[i];
}
//----------------------------------------------------------------------------
float SEDistTriangle3Triangle3f::GetTriangleBary1(int i) const
{
    SE_ASSERT( 0 <= i && i < 3 );

    return m_afTriangleBary1[i];
}
//----------------------------------------------------------------------------
