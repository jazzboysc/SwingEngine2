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
#include "SETriangle3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SETriangle3f::SETriangle3f()
{
    // Œ¥≥ı ºªØ.
}
//----------------------------------------------------------------------------
SETriangle3f::SETriangle3f(const SEVector3f& rV0, 
    const SEVector3f& rV1, const SEVector3f& rV2)
{
    V[0] = rV0;
    V[1] = rV1;
    V[2] = rV2;
}
//----------------------------------------------------------------------------
SETriangle3f::SETriangle3f(const SEVector3f aV[3])
{
    for( int i = 0; i < 3; i++ )
    {
        V[i] = aV[i];
    }
}
//----------------------------------------------------------------------------
float SETriangle3f::GetDistance(const SEVector3f& rQ) const
{
    SEVector3f vec3fDiff = V[0] - rQ;
    SEVector3f vec3fE0 = V[1] - V[0], vec3fE1 = V[2] - V[0];
    float fA00 = vec3fE0.GetSquaredLength();
    float fA01 = vec3fE0.Dot(vec3fE1);
    float fA11 = vec3fE1.GetSquaredLength();
    float fB0 = vec3fDiff.Dot(vec3fE0);
    float fB1 = vec3fDiff.Dot(vec3fE1);
    float fC = vec3fDiff.GetSquaredLength();
    float fDet = SEMath<float>::FAbs(fA00*fA11 - fA01*fA01);
    float fS = fA01*fB1 - fA11*fB0;
    float fT = fA01*fB0 - fA00*fB1;
    float fSqrDist;

    if( fS + fT <= fDet )
    {
        if( fS < 0.0f )
        {
            if( fT < 0.0f )  // region 4
            {
                if( fB0 < 0.0f )
                {
                    if( -fB0 >= fA00 )
                    {
                        fSqrDist = fA00 + 2.0f*fB0 + fC;
                    }
                    else
                    {
                        fSqrDist = fC - fB0*fB0/fA00;
                    }
                }
                else
                {
                    if( fB1 >= 0.0f )
                    {
                        fSqrDist = fC;
                    }
                    else if( -fB1 >= fA11 )
                    {
                        fSqrDist = fA11 + 2.0f*fB1 + fC;
                    }
                    else
                    {
                        fSqrDist = fC - fB1*fB1/fA11;
                    }
                }
            }
            else  // region 3
            {
                if( fB1 >= 0.0f )
                {
                    fSqrDist = fC;
                }
                else if( -fB1 >= fA11 )
                {
                    fSqrDist = fA11 + 2.0f*fB1 + fC;
                }
                else
                {
                    fSqrDist = fC - fB1*fB1/fA11;
                }
            }
        }
        else if( fT < 0.0f )  // region 5
        {
            if( fB0 >= 0.0f )
            {
                fSqrDist = fC;
            }
            else if( -fB0 >= fA00 )
            {
                fSqrDist = fA00 + 2.0f*fB0 + fC;
            }
            else
            {
                fSqrDist = fC - fB0*fB0/fA00;
            }
        }
        else  // region 0
        {
            // minimum at interior point
            float fInvDet = 1.0f / fDet;
            fS *= fInvDet;
            fT *= fInvDet;
            fSqrDist = fS*(fA00*fS + fA01*fT + 2.0f*fB0) +
                fT*(fA01*fS + fA11*fT + 2.0f*fB1) + fC;
        }
    }
    else
    {
        float fTmp0, fTmp1, fNumer, fDenom;

        if( fS < 0.0f )  // region 2
        {
            fTmp0 = fA01 + fB0;
            fTmp1 = fA11 + fB1;
            if( fTmp1 > fTmp0 )
            {
                fNumer = fTmp1 - fTmp0;
                fDenom = fA00 - 2.0f*fA01 + fA11;
                if( fNumer >= fDenom )
                {
                    fSqrDist = fA00 + 2.0f*fB0 + fC;
                }
                else
                {
                    fS = fNumer/fDenom;
                    fT = 1.0f - fS;
                    fSqrDist = fS*(fA00*fS + fA01*fT + 2.0f*fB0) +
                        fT*(fA01*fS + fA11*fT + 2.0f*fB1) + fC;
                }
            }
            else
            {
                if( fTmp1 <= 0.0f )
                {
                    fSqrDist = fA11 + 2.0f*fB1 + fC;
                }
                else if( fB1 >= 0.0f )
                {
                    fSqrDist = fC;
                }
                else
                {
                    fSqrDist = fC - fB1*fB1/fA11;
                }
            }
        }
        else if( fT < 0.0f )  // region 6
        {
            fTmp0 = fA01 + fB1;
            fTmp1 = fA00 + fB0;
            if( fTmp1 > fTmp0 )
            {
                fNumer = fTmp1 - fTmp0;
                fDenom = fA00 - 2.0f*fA01 + fA11;
                if( fNumer >= fDenom )
                {
                    fT = 1.0f;
                    fS = 0.0f;
                    fSqrDist = fA11 + 2.0f*fB1 + fC;
                }
                else
                {
                    fT = fNumer/fDenom;
                    fS = 1.0f - fT;
                    fSqrDist = fS*(fA00*fS + fA01*fT + 2.0f*fB0) +
                        fT*(fA01*fS + fA11*fT + 2.0f*fB1) + fC;
                }
            }
            else
            {
                if( fTmp1 <= 0.0f )
                {
                    fSqrDist = fA00 + 2.0f*fB0 + fC;
                }
                else if( fB0 >= 0.0f )
                {
                    fSqrDist = fC;
                }
                else
                {
                    fSqrDist = fC - fB0*fB0/fA00;
                }
            }
        }
        else  // region 1
        {
            fNumer = fA11 + fB1 - fA01 - fB0;
            if( fNumer <= 0.0f )
            {
                fSqrDist = fA11 + 2.0f*fB1 + fC;
            }
            else
            {
                fDenom = fA00 - 2.0f*fA01 + fA11;
                if( fNumer >= fDenom )
                {
                    fSqrDist = fA00 + 2.0f*fB0 + fC;
                }
                else
                {
                    fS = fNumer/fDenom;
                    fT = 1.0f - fS;
                    fSqrDist = fS*(fA00*fS + fA01*fT + 2.0f*fB0) +
                        fT*(fA01*fS + fA11*fT + 2.0f*fB1) + fC;
                }
            }
        }
    }

    return SEMath<float>::Sqrt(SEMath<float>::FAbs(fSqrDist));
}
//----------------------------------------------------------------------------
