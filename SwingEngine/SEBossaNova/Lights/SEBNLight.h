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

#ifndef Swing_BNLight_H
#define Swing_BNLight_H

#include "SEBossaNovaLIB.h"
#include "SEVector3.h"
#include "SETransformation.h"
#include "SESpectrum.h"
#include "SEBNLightSample.h"
#include "SEVisibilityTester.h"
#include "SEBNScene.h"
#include "SERayDifferential.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20130527
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEBNLight
{
public:
    SEBNLight(const SETransformation& lightToWorld, int sampleCount = 1);
    virtual ~SEBNLight();

    // Sample incoming radiance direction wo in world space based on radiance 
    // receiving position p in world space.
    virtual SESpectrum Sample_L(const SEVector3f& p, float pEpsilon,
        const SEBNLightSample& ls, float time, SEVector3f* wi, float* pdf,
        SEVisibilityTester* vis) const = 0;

    virtual SESpectrum GetPower(const SEBNScene* scene) const = 0;

    virtual bool IsDeltaLight() const = 0;

    virtual SESpectrum Le(const SERayDifferential& r) const;

    virtual float GetPDF(const SEVector3f& p, const SEVector3f& wi) const = 0;

    virtual SESpectrum Sample_L(const SEBNScene* scene, 
        const SEBNLightSample& ls, float u1, float u2, float time, 
        SERay3f* ray, SEVector3f* Ns, float* pdf) const = 0;

public:
    const int SampleCount;

protected:
    const SETransformation LightToWorld;
};

}

#endif