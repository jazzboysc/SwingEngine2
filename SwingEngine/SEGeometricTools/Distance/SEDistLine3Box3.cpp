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
#include "SEDistLine3Box3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDistLine3Box3f::SEDistLine3Box3f(const SELine3f& rLine, const SEBox3f& rBox)
    :
    m_pLine(&rLine), 
    m_pBox(&rBox)
{
    m_fLParam = 0.0f;
}
//----------------------------------------------------------------------------
const SELine3f& SEDistLine3Box3f::GetLine() const
{
    return *m_pLine;
}
//----------------------------------------------------------------------------
const SEBox3f& SEDistLine3Box3f::GetBox() const
{
    return *m_pBox;
}
//----------------------------------------------------------------------------
float SEDistLine3Box3f::Get()
{
    float fSqrDist = GetSquared();

    return SEMath<float>::Sqrt(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistLine3Box3f::GetSquared()
{
    // 计算直线在box坐标体系下的坐标.
    SEVector3f vec3fDiff = m_pLine->Origin - m_pBox->Center;
    SEVector3f vec3fPnt(
        vec3fDiff.Dot(m_pBox->Axis[0]), 
        vec3fDiff.Dot(m_pBox->Axis[1]), 
        vec3fDiff.Dot(m_pBox->Axis[2]));
    SEVector3f vec3fDir(
        m_pLine->Direction.Dot(m_pBox->Axis[0]), 
        m_pLine->Direction.Dot(m_pBox->Axis[1]), 
        m_pLine->Direction.Dot(m_pBox->Axis[2]));

    // 应用反射变换, 使方向向量的分量都非负.
    bool bReflect[3];
    int i;
    for( i = 0; i < 3; i++ )
    {
        if( vec3fDir[i] < 0.0f )
        {
            vec3fPnt[i] = -vec3fPnt[i];
            vec3fDir[i] = -vec3fDir[i];
            bReflect[i] = true;
        }
        else
        {
            bReflect[i] = false;
        }
    }

    float fSqrDistance = 0.0f;
    m_fLParam = 0.0f;  // 最近点处直线参数

    if( vec3fDir.X > 0.0f )
    {
        if( vec3fDir.Y > 0.0f )
        {
            if( vec3fDir.Z > 0.0f )  // (+, +, +)
            {
                CaseNoZeros(vec3fPnt, vec3fDir, fSqrDistance);
            }
            else  // (+, +, 0)
            {
                Case0(0, 1, 2, vec3fPnt, vec3fDir, fSqrDistance);
            }
        }
        else
        {
            if( vec3fDir.Z > 0.0f )  // (+, 0, +)
            {
                Case0(0, 2, 1, vec3fPnt, vec3fDir, fSqrDistance);
            }
            else  // (+, 0, 0)
            {
                Case00(0, 1, 2, vec3fPnt, vec3fDir, fSqrDistance);
            }
        }
    }
    else
    {
        if( vec3fDir.Y > 0.0f )
        {
            if( vec3fDir.Z > 0.0f )  // (0, +, +)
            {
                Case0(1, 2, 0, vec3fPnt, vec3fDir, fSqrDistance);
            }
            else  // (0, +, 0)
            {
                Case00(1, 0, 2, vec3fPnt, vec3fDir, fSqrDistance);
            }
        }
        else
        {
            if( vec3fDir.Z > 0.0f )  // (0, 0, +)
            {
                Case00(2, 0, 1, vec3fPnt, vec3fDir, fSqrDistance);
            }
            else  // (0, 0, 0)
            {
                Case000(vec3fPnt, fSqrDistance);
            }
        }
    }

    // 计算直线最近点.
    m_ClosestPoint0 = m_pLine->Origin + m_fLParam*m_pLine->Direction;

    // 计算box最近点.
    m_ClosestPoint1 = m_pBox->Center;
    for( i = 0; i < 3; i++ )
    {
        // undo之前的反射变换.
        if( bReflect[i] )
        {
            vec3fPnt[i] = -vec3fPnt[i];
        }

        m_ClosestPoint1 += vec3fPnt[i] * m_pBox->Axis[i];
    }

    return fSqrDistance;
}
//----------------------------------------------------------------------------
float SEDistLine3Box3f::Get(float fT, const SEVector3f& rVelocity0, 
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMOrigin = m_pLine->Origin + fT*rVelocity0;
    SEVector3f vec3fMCenter = m_pBox->Center + fT*rVelocity1;
    SELine3f tempMLine(vec3fMOrigin, m_pLine->Direction);
    SEBox3f tempMBox(vec3fMCenter, m_pBox->Axis, m_pBox->Extent);

    return SEDistLine3Box3f(tempMLine, tempMBox).Get();
}
//----------------------------------------------------------------------------
float SEDistLine3Box3f::GetSquared(float fT, const SEVector3f& rVelocity0, 
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMOrigin = m_pLine->Origin + fT*rVelocity0;
    SEVector3f vec3fMCenter = m_pBox->Center + fT*rVelocity1;
    SELine3f tempMLine(vec3fMOrigin, m_pLine->Direction);
    SEBox3f tempMBox(vec3fMCenter, m_pBox->Axis, m_pBox->Extent);

    return SEDistLine3Box3f(tempMLine, tempMBox).GetSquared();
}
//----------------------------------------------------------------------------
float SEDistLine3Box3f::GetLineParameter() const
{
    return m_fLParam;
}
//----------------------------------------------------------------------------
void SEDistLine3Box3f::Face(int i0, int i1, int i2, SEVector3f& rPnt, 
    const SEVector3f& rDir, const SEVector3f& rPmE, 
    float& rfSqrDistance)
{
    SEVector3f vec3fPpE;
    float fLSqr, fInv, fTmp, fParam, fT, fDelta;

    vec3fPpE[i1] = rPnt[i1] + m_pBox->Extent[i1];
    vec3fPpE[i2] = rPnt[i2] + m_pBox->Extent[i2];
    if( rDir[i0]*vec3fPpE[i1] >= rDir[i1]*rPmE[i0] )
    {
        if( rDir[i0]*vec3fPpE[i2] >= rDir[i2]*rPmE[i0] )
        {
            // v[i1] >= -e[i1], v[i2] >= -e[i2] (distance = 0)
            rPnt[i0] = m_pBox->Extent[i0];
            fInv = 1.0f / rDir[i0];
            rPnt[i1] -= rDir[i1]*rPmE[i0]*fInv;
            rPnt[i2] -= rDir[i2]*rPmE[i0]*fInv;
            m_fLParam = -rPmE[i0]*fInv;
        }
        else
        {
            // v[i1] >= -e[i1], v[i2] < -e[i2]
            fLSqr = rDir[i0]*rDir[i0] + rDir[i2]*rDir[i2];
            fTmp = fLSqr*vec3fPpE[i1] - rDir[i1]*(rDir[i0]*rPmE[i0] +
                rDir[i2]*vec3fPpE[i2]);
            if( fTmp <= 2.0f*fLSqr*m_pBox->Extent[i1] )
            {
                fT = fTmp/fLSqr;
                fLSqr += rDir[i1]*rDir[i1];
                fTmp = vec3fPpE[i1] - fT;
                fDelta = rDir[i0]*rPmE[i0] + rDir[i1]*fTmp +
                    rDir[i2]*vec3fPpE[i2];
                fParam = -fDelta/fLSqr;
                rfSqrDistance += rPmE[i0]*rPmE[i0] + fTmp*fTmp +
                    vec3fPpE[i2]*vec3fPpE[i2] + fDelta*fParam;

                m_fLParam = fParam;
                rPnt[i0] = m_pBox->Extent[i0];
                rPnt[i1] = fT - m_pBox->Extent[i1];
                rPnt[i2] = -m_pBox->Extent[i2];
            }
            else
            {
                fLSqr += rDir[i1]*rDir[i1];
                fDelta = rDir[i0]*rPmE[i0] + rDir[i1]*rPmE[i1] +
                    rDir[i2]*vec3fPpE[i2];
                fParam = -fDelta/fLSqr;
                rfSqrDistance += rPmE[i0]*rPmE[i0] + rPmE[i1]*rPmE[i1] +
                    vec3fPpE[i2]*vec3fPpE[i2] + fDelta*fParam;

                m_fLParam = fParam;
                rPnt[i0] = m_pBox->Extent[i0];
                rPnt[i1] = m_pBox->Extent[i1];
                rPnt[i2] = -m_pBox->Extent[i2];
            }
        }
    }
    else
    {
        if( rDir[i0]*vec3fPpE[i2] >= rDir[i2]*rPmE[i0] )
        {
            // v[i1] < -e[i1], v[i2] >= -e[i2]
            fLSqr = rDir[i0]*rDir[i0] + rDir[i1]*rDir[i1];
            fTmp = fLSqr*vec3fPpE[i2] - rDir[i2]*(rDir[i0]*rPmE[i0] +
                rDir[i1]*vec3fPpE[i1]);
            if( fTmp <= 2.0f*fLSqr*m_pBox->Extent[i2] )
            {
                fT = fTmp/fLSqr;
                fLSqr += rDir[i2]*rDir[i2];
                fTmp = vec3fPpE[i2] - fT;
                fDelta = rDir[i0]*rPmE[i0] + rDir[i1]*vec3fPpE[i1] +
                    rDir[i2]*fTmp;
                fParam = -fDelta/fLSqr;
                rfSqrDistance += rPmE[i0]*rPmE[i0] + vec3fPpE[i1]*vec3fPpE[i1]
                    + fTmp*fTmp + fDelta*fParam;

                m_fLParam = fParam;
                rPnt[i0] = m_pBox->Extent[i0];
                rPnt[i1] = -m_pBox->Extent[i1];
                rPnt[i2] = fT - m_pBox->Extent[i2];
            }
            else
            {
                fLSqr += rDir[i2]*rDir[i2];
                fDelta = rDir[i0]*rPmE[i0] + rDir[i1]*vec3fPpE[i1] +
                    rDir[i2]*rPmE[i2];
                fParam = -fDelta/fLSqr;
                rfSqrDistance += rPmE[i0]*rPmE[i0] + vec3fPpE[i1]*vec3fPpE[i1]
                    + rPmE[i2]*rPmE[i2] + fDelta*fParam;

                m_fLParam = fParam;
                rPnt[i0] = m_pBox->Extent[i0];
                rPnt[i1] = -m_pBox->Extent[i1];
                rPnt[i2] = m_pBox->Extent[i2];
            }
        }
        else
        {
            // v[i1] < -e[i1], v[i2] < -e[i2]
            fLSqr = rDir[i0]*rDir[i0]+rDir[i2]*rDir[i2];
            fTmp = fLSqr*vec3fPpE[i1] - rDir[i1]*(rDir[i0]*rPmE[i0] +
                rDir[i2]*vec3fPpE[i2]);
            if( fTmp >= 0.0f )
            {
                // v[i1]-edge is closest
                if( fTmp <= 2.0f*fLSqr*m_pBox->Extent[i1] )
                {
                    fT = fTmp/fLSqr;
                    fLSqr += rDir[i1]*rDir[i1];
                    fTmp = vec3fPpE[i1] - fT;
                    fDelta = rDir[i0]*rPmE[i0] + rDir[i1]*fTmp +
                        rDir[i2]*vec3fPpE[i2];
                    fParam = -fDelta/fLSqr;
                    rfSqrDistance += rPmE[i0]*rPmE[i0] + fTmp*fTmp +
                        vec3fPpE[i2]*vec3fPpE[i2] + fDelta*fParam;

                    m_fLParam = fParam;
                    rPnt[i0] = m_pBox->Extent[i0];
                    rPnt[i1] = fT - m_pBox->Extent[i1];
                    rPnt[i2] = -m_pBox->Extent[i2];
                }
                else
                {
                    fLSqr += rDir[i1]*rDir[i1];
                    fDelta = rDir[i0]*rPmE[i0] + rDir[i1]*rPmE[i1] +
                        rDir[i2]*vec3fPpE[i2];
                    fParam = -fDelta/fLSqr;
                    rfSqrDistance += rPmE[i0]*rPmE[i0] + rPmE[i1]*rPmE[i1]
                        + vec3fPpE[i2]*vec3fPpE[i2] + fDelta*fParam;

                    m_fLParam = fParam;
                    rPnt[i0] = m_pBox->Extent[i0];
                    rPnt[i1] = m_pBox->Extent[i1];
                    rPnt[i2] = -m_pBox->Extent[i2];
                }

                return;
            }

            fLSqr = rDir[i0]*rDir[i0] + rDir[i1]*rDir[i1];
            fTmp = fLSqr*vec3fPpE[i2] - rDir[i2]*(rDir[i0]*rPmE[i0] +
                rDir[i1]*vec3fPpE[i1]);
            if( fTmp >= 0.0f )
            {
                // v[i2]-edge is closest
                if( fTmp <= 2.0f*fLSqr*m_pBox->Extent[i2])
                {
                    fT = fTmp/fLSqr;
                    fLSqr += rDir[i2]*rDir[i2];
                    fTmp = vec3fPpE[i2] - fT;
                    fDelta = rDir[i0]*rPmE[i0] + rDir[i1]*vec3fPpE[i1] +
                        rDir[i2]*fTmp;
                    fParam = -fDelta/fLSqr;
                    rfSqrDistance += rPmE[i0]*rPmE[i0] + 
                        vec3fPpE[i1]*vec3fPpE[i1] + fTmp*fTmp + fDelta*fParam;

                    m_fLParam = fParam;
                    rPnt[i0] = m_pBox->Extent[i0];
                    rPnt[i1] = -m_pBox->Extent[i1];
                    rPnt[i2] = fT - m_pBox->Extent[i2];
                }
                else
                {
                    fLSqr += rDir[i2]*rDir[i2];
                    fDelta = rDir[i0]*rPmE[i0] + rDir[i1]*vec3fPpE[i1] +
                        rDir[i2]*rPmE[i2];
                    fParam = -fDelta/fLSqr;
                    rfSqrDistance += rPmE[i0]*rPmE[i0] + 
                        vec3fPpE[i1]*vec3fPpE[i1] + rPmE[i2]*rPmE[i2] + 
                        fDelta*fParam;

                    m_fLParam = fParam;
                    rPnt[i0] = m_pBox->Extent[i0];
                    rPnt[i1] = -m_pBox->Extent[i1];
                    rPnt[i2] = m_pBox->Extent[i2];
                }

                return;
            }

            // (v[i1], v[i2])-corner is closest
            fLSqr += rDir[i2]*rDir[i2];
            fDelta = rDir[i0]*rPmE[i0] + rDir[i1]*vec3fPpE[i1] +
                rDir[i2]*vec3fPpE[i2];
            fParam = -fDelta/fLSqr;
            rfSqrDistance += rPmE[i0]*rPmE[i0] + vec3fPpE[i1]*vec3fPpE[i1] +
                vec3fPpE[i2]*vec3fPpE[i2] + fDelta*fParam;

            m_fLParam = fParam;
            rPnt[i0] = m_pBox->Extent[i0];
            rPnt[i1] = -m_pBox->Extent[i1];
            rPnt[i2] = -m_pBox->Extent[i2];
        }
    }
}
//----------------------------------------------------------------------------
void SEDistLine3Box3f::CaseNoZeros(SEVector3f& rPnt, const SEVector3f& rDir, 
    float& rfSqrDistance)
{
    SEVector3f vec3fPmE(
        rPnt.X - m_pBox->Extent[0], 
        rPnt.Y - m_pBox->Extent[1], 
        rPnt.Z - m_pBox->Extent[2]);

    float fProdDxPy = rDir.X*vec3fPmE.Y;
    float fProdDyPx = rDir.Y*vec3fPmE.X;
    float fProdDzPx, fProdDxPz, fProdDzPy, fProdDyPz;

    if( fProdDyPx >= fProdDxPy )
    {
        fProdDzPx = rDir.Z*vec3fPmE.X;
        fProdDxPz = rDir.X*vec3fPmE.Z;
        if( fProdDzPx >= fProdDxPz )
        {
            // line intersects x = e0
            Face(0, 1, 2, rPnt, rDir, vec3fPmE, rfSqrDistance);
        }
        else
        {
            // line intersects z = e2
            Face(2, 0, 1, rPnt, rDir, vec3fPmE, rfSqrDistance);
        }
    }
    else
    {
        fProdDzPy = rDir.Z*vec3fPmE.Y;
        fProdDyPz = rDir.Y*vec3fPmE.Z;
        if( fProdDzPy >= fProdDyPz )
        {
            // line intersects y = e1
            Face(1, 2, 0, rPnt, rDir, vec3fPmE, rfSqrDistance);
        }
        else
        {
            // line intersects z = e2
            Face(2, 0, 1, rPnt, rDir, vec3fPmE, rfSqrDistance);
        }
    }
}
//----------------------------------------------------------------------------
void SEDistLine3Box3f::Case0(int i0, int i1, int i2, SEVector3f& rPnt, 
    const SEVector3f& rDir, float& rfSqrDistance)
{
    float fPmE0 = rPnt[i0] - m_pBox->Extent[i0];
    float fPmE1 = rPnt[i1] - m_pBox->Extent[i1];
    float fProd0 = rDir[i1]*fPmE0;
    float fProd1 = rDir[i0]*fPmE1;
    float fDelta, fInvLSqr, fInv;

    if( fProd0 >= fProd1 )
    {
        // line intersects P[i0] = e[i0]
        rPnt[i0] = m_pBox->Extent[i0];

        float fPpE1 = rPnt[i1] + m_pBox->Extent[i1];
        fDelta = fProd0 - rDir[i0]*fPpE1;
        if( fDelta >= 0.0f )
        {
            fInvLSqr = 1.0f / (rDir[i0]*rDir[i0]+rDir[i1]*rDir[i1]);
            rfSqrDistance += fDelta*fDelta*fInvLSqr;
            rPnt[i1] = -m_pBox->Extent[i1];
            m_fLParam = -(rDir[i0]*fPmE0+rDir[i1]*fPpE1)*fInvLSqr;
        }
        else
        {
            fInv = 1.0f / rDir[i0];
            rPnt[i1] -= fProd0*fInv;
            m_fLParam = -fPmE0*fInv;
        }
    }
    else
    {
        // line intersects P[i1] = e[i1]
        rPnt[i1] = m_pBox->Extent[i1];

        float fPpE0 = rPnt[i0] + m_pBox->Extent[i0];
        fDelta = fProd1 - rDir[i1]*fPpE0;
        if( fDelta >= 0.0f )
        {
            fInvLSqr = 1.0f / (rDir[i0]*rDir[i0]+rDir[i1]*rDir[i1]);
            rfSqrDistance += fDelta*fDelta*fInvLSqr;
            rPnt[i0] = -m_pBox->Extent[i0];
            m_fLParam = -(rDir[i0]*fPpE0+rDir[i1]*fPmE1)*fInvLSqr;
        }
        else
        {
            fInv = 1.0f / rDir[i1];
            rPnt[i0] -= fProd1*fInv;
            m_fLParam = -fPmE1*fInv;
        }
    }

    if( rPnt[i2] < -m_pBox->Extent[i2] )
    {
        fDelta = rPnt[i2] + m_pBox->Extent[i2];
        rfSqrDistance += fDelta*fDelta;
        rPnt[i2] = -m_pBox->Extent[i2];
    }
    else if( rPnt[i2] > m_pBox->Extent[i2] )
    {
        fDelta = rPnt[i2] - m_pBox->Extent[i2];
        rfSqrDistance += fDelta*fDelta;
        rPnt[i2] = m_pBox->Extent[i2];
    }
}
//----------------------------------------------------------------------------
void SEDistLine3Box3f::Case00(int i0, int i1, int i2, SEVector3f& rPnt, 
    const SEVector3f& rDir,  float& rfSqrDistance)
{
    float fDelta;

    m_fLParam = (m_pBox->Extent[i0] - rPnt[i0])/rDir[i0];

    rPnt[i0] = m_pBox->Extent[i0];

    if( rPnt[i1] < -m_pBox->Extent[i1] )
    {
        fDelta = rPnt[i1] + m_pBox->Extent[i1];
        rfSqrDistance += fDelta*fDelta;
        rPnt[i1] = -m_pBox->Extent[i1];
    }
    else if( rPnt[i1] > m_pBox->Extent[i1] )
    {
        fDelta = rPnt[i1] - m_pBox->Extent[i1];
        rfSqrDistance += fDelta*fDelta;
        rPnt[i1] = m_pBox->Extent[i1];
    }

    if( rPnt[i2] < -m_pBox->Extent[i2] )
    {
        fDelta = rPnt[i2] + m_pBox->Extent[i2];
        rfSqrDistance += fDelta*fDelta;
        rPnt[i2] = -m_pBox->Extent[i2];
    }
    else if( rPnt[i2] > m_pBox->Extent[i2] )
    {
        fDelta = rPnt[i2] - m_pBox->Extent[i2];
        rfSqrDistance += fDelta*fDelta;
        rPnt[i2] = m_pBox->Extent[i2];
    }
}
//----------------------------------------------------------------------------
void SEDistLine3Box3f::Case000(SEVector3f& rPnt, float& rfSqrDistance)
{
    float fDelta;

    if( rPnt.X < -m_pBox->Extent[0] )
    {
        fDelta = rPnt.X + m_pBox->Extent[0];
        rfSqrDistance += fDelta*fDelta;
        rPnt.X = -m_pBox->Extent[0];
    }
    else if( rPnt.X > m_pBox->Extent[0] )
    {
        fDelta = rPnt.X - m_pBox->Extent[0];
        rfSqrDistance += fDelta*fDelta;
        rPnt.X = m_pBox->Extent[0];
    }

    if( rPnt.Y < -m_pBox->Extent[1] )
    {
        fDelta = rPnt.Y + m_pBox->Extent[1];
        rfSqrDistance += fDelta*fDelta;
        rPnt.Y = -m_pBox->Extent[1];
    }
    else if( rPnt.Y > m_pBox->Extent[1] )
    {
        fDelta = rPnt.Y - m_pBox->Extent[1];
        rfSqrDistance += fDelta*fDelta;
        rPnt.Y = m_pBox->Extent[1];
    }

    if( rPnt.Z < -m_pBox->Extent[2] )
    {
        fDelta = rPnt.Z + m_pBox->Extent[2];
        rfSqrDistance += fDelta*fDelta;
        rPnt.Z = -m_pBox->Extent[2];
    }
    else if( rPnt.Z > m_pBox->Extent[2] )
    {
        fDelta = rPnt.Z - m_pBox->Extent[2];
        rfSqrDistance += fDelta*fDelta;
        rPnt.Z = m_pBox->Extent[2];
    }
}
//----------------------------------------------------------------------------
