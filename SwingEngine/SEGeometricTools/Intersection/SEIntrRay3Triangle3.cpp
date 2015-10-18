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
#include "SEIntrRay3Triangle3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEIntrRay3Triangle3f::SEIntrRay3Triangle3f(const SERay3f& rRay,
    const SETriangle3f& rTriangle)
    :
    m_pRay(&rRay),
    m_pTriangle(&rTriangle)
{
}
//----------------------------------------------------------------------------
const SERay3f& SEIntrRay3Triangle3f::GetRay() const
{
    return *m_pRay;
}
//----------------------------------------------------------------------------
const SETriangle3f& SEIntrRay3Triangle3f::GetTriangle() const
{
    return *m_pTriangle;
}
//----------------------------------------------------------------------------
bool SEIntrRay3Triangle3f::Test()
{
    // 计算offset origin, edges, normal.
    SEVector3f vec3fDiff = m_pRay->Origin - m_pTriangle->V[0];
    SEVector3f vec3fEdge1 = m_pTriangle->V[1] - m_pTriangle->V[0];
    SEVector3f vec3fEdge2 = m_pTriangle->V[2] - m_pTriangle->V[0];
    SEVector3f vec3fNormal = vec3fEdge1.Cross(vec3fEdge2);

    // 根据射线与三角形所在面交点,建立等式: O + t*D = b0*V0 + b1*V1 + b2*V2,
    // 即, O + t*D = (1 - b1 - b2)*V0 + b1*V1 + b2*V2,
    // 即, (O - V0) + t*D = b1*(V1 - V0) + b2*(V2 - V0),
    // 其中O为射线原点,D为射线方向.
    // 解线性方程组 Q + t*D = b1*E1 + b2*E2 (Q = vec3fDiff, D = ray direction,
    // E1 = vec3fEdge1, E2 = vec3fEdge2, N = Cross(E1,E2)) 通过计算:
    //   |Dot(D,N)|*b1 = sign(Dot(D,N))*Dot(D,Cross(Q,E2))
    //   |Dot(D,N)|*b2 = sign(Dot(D,N))*Dot(D,Cross(E1,Q))
    //   |Dot(D,N)|*t = -sign(Dot(D,N))*Dot(Q,N)
    // 其中sign()为取符号函数.
    float fDdN = m_pRay->Direction.Dot(vec3fNormal);
    float fSign;
    if( fDdN > SEMathf::ZERO_TOLERANCE )
    {
        fSign = 1.0f;
    }
    else if( fDdN < -SEMathf::ZERO_TOLERANCE )
    {
        fSign = -1.0f;
        fDdN = -fDdN;
    }
    else
    {
        // 射线与三角形平行,即使相交也规定为不相交.
        return false;
    }

    float fDdQxE2 = fSign*m_pRay->Direction.Dot(vec3fDiff.Cross(vec3fEdge2));
    if( fDdQxE2 >= 0.0f )
    {
        float fDdE1xQ = fSign*m_pRay->Direction.Dot(vec3fEdge1.Cross(
            vec3fDiff));
        if( fDdE1xQ >= 0.0f )
        {
            if( fDdQxE2 + fDdE1xQ <= fDdN )
            {
                // 直线与三角形相交,检查射线是否相交.
                float fQdN = -fSign*vec3fDiff.Dot(vec3fNormal);
                if( fQdN >= 0.0f )
                {
                    // 射线与三角形相交.
                    return true;
                }
                // else: t < 0, 不相交
            }
            // else: b1+b2 > 1, 不相交
        }
        // else: b2 < 0, 不相交
    }
    // else: b1 < 0, 不相交

    return false;
}
//----------------------------------------------------------------------------
bool SEIntrRay3Triangle3f::Find()
{
    // 计算offset origin, edges, normal.
    SEVector3f vec3fDiff = m_pRay->Origin - m_pTriangle->V[0];
    SEVector3f vec3fEdge1 = m_pTriangle->V[1] - m_pTriangle->V[0];
    SEVector3f vec3fEdge2 = m_pTriangle->V[2] - m_pTriangle->V[0];
    SEVector3f vec3fNormal = vec3fEdge1.Cross(vec3fEdge2);

    // 根据射线与三角形所在面交点,建立等式: O + t*D = b0*V0 + b1*V1 + b2*V2,
    // 即, O + t*D = (1 - b2 - b2)*V0 + b1*V1 + b2*V2,
    // 即, (O - V0) + t*D = b1*(V1 - V0) + b2*(V2 - V0),
    // 其中O为射线原点,D为射线方向.
    // 解线性方程组 Q + t*D = b1*E1 + b2*E2 (Q = vec3fDiff, D = ray direction,
    // E1 = vec3fEdge1, E2 = vec3fEdge2, N = Cross(E1,E2)) 通过计算:
    //   |Dot(D,N)|*b1 = sign(Dot(D,N))*Dot(D,Cross(Q,E2))
    //   |Dot(D,N)|*b2 = sign(Dot(D,N))*Dot(D,Cross(E1,Q))
    //   |Dot(D,N)|*t = -sign(Dot(D,N))*Dot(Q,N)
    // 其中sign()为取符号函数.
    float fDdN = m_pRay->Direction.Dot(vec3fNormal);
    float fSign;
    if( fDdN > SEMathf::ZERO_TOLERANCE )
    {
        fSign = 1.0f;
    }
    else if( fDdN < -SEMathf::ZERO_TOLERANCE )
    {
        fSign = -1.0f;
        fDdN = -fDdN;
    }
    else
    {
        // 射线与三角形平行,即使相交也规定为不相交.
        return false;
    }

    float fDdQxE2 = fSign*m_pRay->Direction.Dot(vec3fDiff.Cross(vec3fEdge2));
    if( fDdQxE2 >= 0.0f )
    {
        float fDdE1xQ = fSign*m_pRay->Direction.Dot(vec3fEdge1.Cross(
            vec3fDiff));
        if( fDdE1xQ >= 0.0f )
        {
            if( fDdQxE2 + fDdE1xQ <= fDdN )
            {
                // 直线与三角形相交,检查射线是否相交.
                float fQdN = -fSign*vec3fDiff.Dot(vec3fNormal);
                if( fQdN >= 0.0f )
                {
                    // 射线与三角形相交.
                    float fInv = 1.0f / fDdN;
                    m_fRayT = fQdN * fInv;
                    m_fTriB1 = fDdQxE2 * fInv;
                    m_fTriB2 = fDdE1xQ * fInv;
                    m_fTriB0 = 1.0f - m_fTriB1 - m_fTriB2;

                    return true;
                }
                // else: t < 0, 不相交
            }
            // else: b1+b2 > 1, 不相交
        }
        // else: b2 < 0, 不相交
    }
    // else: b1 < 0, 不相交

    return false;
}
//----------------------------------------------------------------------------
float SEIntrRay3Triangle3f::GetRayT() const
{
    return m_fRayT;
}
//----------------------------------------------------------------------------
float SEIntrRay3Triangle3f::GetTriB0() const
{
    return m_fTriB0;
}
//----------------------------------------------------------------------------
float SEIntrRay3Triangle3f::GetTriB1() const
{
    return m_fTriB1;
}
//----------------------------------------------------------------------------
float SEIntrRay3Triangle3f::GetTriB2() const
{
    return m_fTriB2;
}
//----------------------------------------------------------------------------
