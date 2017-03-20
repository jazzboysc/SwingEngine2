// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017
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

#ifndef Swing_BNProjectiveCamera_H
#define Swing_BNProjectiveCamera_H

#include "SEBossaNovaLIB.h"
#include "SEBNCamera.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20130520
//----------------------------------------------------------------------------
class SEBNProjectiveCamera : public SEBNCamera
{
    SE_DECLARE_RTTI;

public:
    SEBNProjectiveCamera(float sopen, float sclose, SEFilm* film, float lensr,
        float focald);
    virtual ~SEBNProjectiveCamera();

    // View frustum.
    enum
    {
        VF_DMIN     = 0,  // near
        VF_DMAX     = 1,  // far
        VF_UMIN     = 2,  // bottom
        VF_UMAX     = 3,  // top
        VF_RMIN     = 4,  // left
        VF_RMAX     = 5,  // right
        VF_COUNT = 6
    };

    // Frustum accessors.
    void SetFrustum(float rmin, float rmax, float umin, float umax,
        float dmin, float dmax);
    void SetFrustum(float upFovDegrees, float aspectRatio, float dmin,
        float dmax);
    void GetFrustum(float& rmin, float& rmax, float& umin, float& umax, 
        float& dmin, float& dmax) const;
    bool GetFrustum(float& upFovDegrees, float& aspectRatio, float& dmin, 
        float& dmax) const;

protected:
    // Lens parameters.
    float LensRadius, FocalDistance;

    // View frustum parameters.(near, far, bottom, top, left, right).
    float Frustum[VF_COUNT];

    // FocalDistanceToDMin = FocalDistance / Frustum[VF_DMIN].
    float FocalDistanceToDMin;
};

typedef SESmartPointer<SEBNProjectiveCamera> SEBNProjectiveCameraPtr;

#include "SEBNProjectiveCamera.inl"

}

#endif