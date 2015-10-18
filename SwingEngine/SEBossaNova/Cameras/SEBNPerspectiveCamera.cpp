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
#include "SEBNPerspectiveCamera.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SEBNPerspectiveCamera, SEBNProjectiveCamera);

//----------------------------------------------------------------------------
SEBNPerspectiveCamera::SEBNPerspectiveCamera(float sopen, float sclose, 
    SEFilm* film, float lensr, float focald)
    :
    SEBNProjectiveCamera(sopen, sclose, film, lensr, focald)
{
}
//----------------------------------------------------------------------------
SEBNPerspectiveCamera::~SEBNPerspectiveCamera()
{
}
//----------------------------------------------------------------------------
float SEBNPerspectiveCamera::GenerateRay(const SEBNCameraSample& sample, 
    SERay3f* ray) const
{
    float xWeight = sample.ImageX * InvXResolution;
    float yWeight = (YResolution - sample.ImageY) * InvYResolution;

    float viewX = (1.0f - xWeight)*Frustum[VF_RMIN] + 
        xWeight*Frustum[VF_RMAX];
    float viewY = (1.0f - yWeight)*Frustum[VF_UMIN] + 
        yWeight*Frustum[VF_UMAX];

    // The ray is E + tW, t >= 0.
    // W = (Q - E)/|Q - E|, Q = E + n*D + Xv*R + Yv*U.
    ray->Origin = Location;
    ray->Direction = Frustum[VF_DMIN]*DVector + viewX*RVector + viewY*UVector;

    // Modify ray for depth of field.
    if( LensRadius > 0.0f )
    {
        // Sample point on lens.
        float lensU, lensV;
        SEMonteCarlo::ConcentricSampleDisk(sample.LensU, sample.LensV, 
            &lensU, &lensV);
        lensU *= LensRadius;
        lensV *= LensRadius;

        // Compute point on plane of focus in world space.
        float viewXFocus = FocalDistanceToDMin * viewX;
        float viewYFocus = FocalDistanceToDMin * viewY;
        SEVector3f pFocus = Location + FocalDistance*DVector + 
            viewXFocus*RVector + viewYFocus*UVector;

        // Update ray for effect of lens.
        ray->Origin += lensU*RVector + lensV*UVector;
        ray->Direction = pFocus - ray->Origin;
    }

    ray->Direction.Normalize();
    ray->Time = sample.Time;
    ray->MinT = 0.0f;
    ray->MaxT = SEMathf::MAX_REAL;
    ray->Depth = 0;

    return 1.0f;
}
//----------------------------------------------------------------------------