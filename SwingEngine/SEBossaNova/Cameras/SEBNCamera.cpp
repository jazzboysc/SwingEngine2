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
#include "SEBNCamera.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SEBNCamera, SEObject);

//----------------------------------------------------------------------------
SEBNCamera::SEBNCamera(float sopen, float sclose, SEFilm* film)
    :
    ShutterOpen(sopen),
    ShutterClose(sclose),
    XResolution((float)film->XResolution),
    YResolution((float)film->YResolution),
    InvXResolution(1.0f / XResolution),
    InvYResolution(1.0f / YResolution),
    Film(film)
{
    SetFrame(SEVector3f::ZERO, SEVector3f::UNIT_X, SEVector3f::UNIT_Y, 
        SEVector3f::UNIT_Z);
}
//----------------------------------------------------------------------------
SEBNCamera::~SEBNCamera()
{
    SE_DELETE Film;
    Film = 0;
}
//----------------------------------------------------------------------------
float SEBNCamera::GenerateRayDifferential(const SEBNCameraSample& sample, 
    SERayDifferential* rd) const
{
    float wt = GenerateRay(sample, rd);

    // Find ray after shifting one pixel in the x direction.
    SEBNCameraSample sshift = sample;
    ++(sshift.ImageX);
    SERay3f rx;
    float wtx = GenerateRay(sshift, &rx);
    rd->RxOrigin = rx.Origin;
    rd->RxDirection = rx.Direction;

    // Find ray after shifting one pixel in the y direction.
    --(sshift.ImageX);
    ++(sshift.ImageY);
    SERay3f ry;
    float wty = GenerateRay(sshift, &ry);
    rd->RyOrigin = ry.Origin;
    rd->RyDirection = ry.Direction;

    // BNDebug.
    if( wtx == 0.0f || wty == 0.0f )
    {
        return 0.0f;
    }

    rd->HasDifferentials = true;

    return wt;
}
//----------------------------------------------------------------------------
void SEBNCamera::SetFrame(const SEVector3f& location, const SEVector3f& 
    rVector, const SEVector3f& uVector, const SEVector3f& dVector)
{
    Location = location;
    SetAxes(rVector, uVector, dVector);
}
//----------------------------------------------------------------------------
void SEBNCamera::SetLocation(const SEVector3f& location)
{
    Location = location;
}
//----------------------------------------------------------------------------
void SEBNCamera::SetAxes(const SEVector3f& rVector, const SEVector3f& uVector,
    const SEVector3f& dVector)
{
    RVector = rVector;
    UVector = uVector;
    DVector = dVector;

    float absDet = SEMathf::FAbs(DVector.Dot(RVector.Cross(UVector)));
    if( SEMathf::FAbs(1.0f - absDet) > 0.01f )
    {
        SEVector3f::Orthonormalize(RVector, UVector, DVector);
    }
}
//----------------------------------------------------------------------------