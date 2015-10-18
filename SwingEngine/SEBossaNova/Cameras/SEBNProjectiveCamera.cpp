// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015
//
// This part of Swing Engine is based on PBRT.
/*
    pbrt source code Copyright(c) 1998-2012 Matt Pharr and Greg Humphreys.

    This file is part of pbrt.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

#include "SEBossaNovaPCH.h"
#include "SEBNProjectiveCamera.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SEBNProjectiveCamera, SEBNCamera);

//----------------------------------------------------------------------------
SEBNProjectiveCamera::SEBNProjectiveCamera(float sopen, float sclose, 
     SEFilm* film, float lensr, float focald)
    :
    SEBNCamera(sopen, sclose, film),
    LensRadius(lensr),
    FocalDistance(focald)
{
    SetFrustum(-0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 2.0f);
    FocalDistanceToDMin = FocalDistance / Frustum[VF_DMIN];
}
//----------------------------------------------------------------------------
SEBNProjectiveCamera::~SEBNProjectiveCamera()
{
}
//----------------------------------------------------------------------------
void SEBNProjectiveCamera::SetFrustum(float rmin, float rmax, float umin, 
    float umax, float dmin, float dmax)
{
    Frustum[VF_DMIN] = dmin;
    Frustum[VF_DMAX] = dmax;
    Frustum[VF_UMIN] = umin;
    Frustum[VF_UMAX] = umax;
    Frustum[VF_RMIN] = rmin;
    Frustum[VF_RMAX] = rmax;
    FocalDistanceToDMin = FocalDistance / Frustum[VF_DMIN];
}
//----------------------------------------------------------------------------
void SEBNProjectiveCamera::SetFrustum(float upFovDegrees, float aspectRatio, 
    float dmin, float dmax)
{
    float halfAngleRadians = 0.5f * upFovDegrees * SEMathf::DEG_TO_RAD;
    Frustum[VF_UMAX] = dmin * SEMathf::Tan(halfAngleRadians);
    Frustum[VF_RMAX] = aspectRatio * Frustum[VF_UMAX];
    Frustum[VF_UMIN] = -Frustum[VF_UMAX];
    Frustum[VF_RMIN] = -Frustum[VF_RMAX];
    Frustum[VF_DMIN] = dmin;
    Frustum[VF_DMAX] = dmax;
    FocalDistanceToDMin = FocalDistance / Frustum[VF_DMIN];
}
//----------------------------------------------------------------------------
void SEBNProjectiveCamera::GetFrustum(float& rmin, float& rmax, float& umin,
    float& umax, float& dmin, float& dmax) const
{
    dmin = Frustum[VF_DMIN];
    dmax = Frustum[VF_DMAX];
    umin = Frustum[VF_UMIN];
    umax = Frustum[VF_UMAX];
    rmin = Frustum[VF_RMIN];
    rmax = Frustum[VF_RMAX];
}
//----------------------------------------------------------------------------
bool SEBNProjectiveCamera::GetFrustum(float& upFovDegrees, float& aspectRatio,
    float& dmin, float& dmax) const
{
    if( Frustum[VF_RMIN] == -Frustum[VF_RMAX] && 
        Frustum[VF_UMIN] == -Frustum[VF_UMAX] )
    {
        float fTemp = Frustum[VF_UMAX] / Frustum[VF_DMIN];
        upFovDegrees = 2.0f * SEMathf::ATan(fTemp) * SEMathf::RAD_TO_DEG;
        aspectRatio = Frustum[VF_RMAX] / Frustum[VF_UMAX];
        dmin = Frustum[VF_DMIN];
        dmax = Frustum[VF_DMAX];

        return true;
    }

    return false;
}
//----------------------------------------------------------------------------