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

#ifndef Swing_BNCamera_H
#define Swing_BNCamera_H

#include "SEBossaNovaLIB.h"
#include "SERay3.h"
#include "SEVector3.h"
#include "SEFilm.h"
#include "SEBNSampler.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20130520
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEBNCamera : public SEObject
{
    SE_DECLARE_RTTI;

public:
    SEBNCamera(float sopen, float sclose, SEFilm* film);
    virtual ~SEBNCamera();

    // Ray generation interface.
    virtual float GenerateRay(const SEBNCameraSample& sample, 
        SERay3f* ray) const = 0;
    virtual float GenerateRayDifferential(const SEBNCameraSample& sample, 
        SERayDifferential* rd) const;

    // Camera E;R,U,D coordinate system stuff.
    void SetFrame(const SEVector3f& location, const SEVector3f& rVector,
        const SEVector3f& uVector, const SEVector3f& dVector);
    void SetLocation(const SEVector3f& location);
    void SetAxes(const SEVector3f& rVector, const SEVector3f& uVector, 
        const SEVector3f& dVector);

    inline SEVector3f GetLocation() const;
    inline SEVector3f GetRVector() const;
    inline SEVector3f GetUVector() const;
    inline SEVector3f GetDVector() const;

public:
    // Camera parameters.
    const float ShutterOpen, ShutterClose;
    const float XResolution, YResolution;
    const float InvXResolution, InvYResolution;
    SEFilm* Film;

protected:
    // Camera E;R,U,D coordinate system.
    SEVector3f Location, RVector, UVector, DVector;
};

typedef SESmartPointer<SEBNCamera> SEBNCameraPtr;

#include "SEBNCamera.inl"

}

#endif