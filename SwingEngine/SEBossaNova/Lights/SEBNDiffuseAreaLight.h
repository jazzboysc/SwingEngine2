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

#ifndef Swing_BNDiffuseAreaLight_H
#define Swing_BNDiffuseAreaLight_H

#include "SEBossaNovaLIB.h"
#include "SEBNAreaLight.h"
#include "SEBNShapeSet.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20130527
//----------------------------------------------------------------------------
class SEBNDiffuseAreaLight : public SEBNAreaLight
{
public:
    SEBNDiffuseAreaLight(const SETransformation& lightToWorld,
        const SESpectrum& le, int sampleCount, SEBNShape* shape);
    ~SEBNDiffuseAreaLight();

    SESpectrum L(const SEVector3f& p, const SEVector3f& n, 
        const SEVector3f& w) const;

    SESpectrum GetPower(const SEBNScene* scene) const;

    bool IsDeltaLight() const;

    float GetPDF(const SEVector3f& p, const SEVector3f& wi) const;

    // Sample incoming radiance direction wo in world space based on radiance 
    // receiving position p in world space.
    SESpectrum Sample_L(const SEVector3f& p, float pEpsilon, 
        const SEBNLightSample& ls, float time, SEVector3f* wi, float* pdf, 
        SEVisibilityTester* visibility) const;

    SESpectrum Sample_L(const SEBNScene* scene, const SEBNLightSample& ls, 
        float u1, float u2, float time, SERay3f* ray, SEVector3f* Ns, 
        float* pdf) const;

protected:
    SESpectrum Lemit;
    SEBNShapeSet* ShapeSet;
    float Area;
};

}

#endif