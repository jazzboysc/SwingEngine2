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
#include "SEBNDiffuseAreaLight.h"
#include "SEMath.h"
#include "SEMonteCarlo.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEBNDiffuseAreaLight::SEBNDiffuseAreaLight(
    const SETransformation& lightToWorld, const SESpectrum& le, 
    int sampleCount, SEBNShape* shape)
    :
    SEBNAreaLight(lightToWorld, sampleCount)
{
    Lemit = le;
    ShapeSet = SE_NEW SEBNShapeSet(shape);
    Area = ShapeSet->GetArea();
}
//----------------------------------------------------------------------------
SEBNDiffuseAreaLight::~SEBNDiffuseAreaLight()
{
    SE_DELETE ShapeSet;
}
//----------------------------------------------------------------------------
SESpectrum SEBNDiffuseAreaLight::L(const SEVector3f&, const SEVector3f& n, 
    const SEVector3f& w) const
{
    float fDot = n.Dot(w);
    return fDot > 0.0f ? Lemit : 0.0f;
}
//----------------------------------------------------------------------------
SESpectrum SEBNDiffuseAreaLight::GetPower(const SEBNScene*) const
{
    return Lemit * Area * SEMathf::PI;
}
//----------------------------------------------------------------------------
bool SEBNDiffuseAreaLight::IsDeltaLight() const
{
    return false;
}
//----------------------------------------------------------------------------
float SEBNDiffuseAreaLight::GetPDF(const SEVector3f& p, 
    const SEVector3f& wi) const
{
    return ShapeSet->GetPDF(p, wi);
}
//----------------------------------------------------------------------------
SESpectrum SEBNDiffuseAreaLight::Sample_L(const SEVector3f& p, float pEpsilon, 
    const SEBNLightSample& ls, float time, SEVector3f* wi, float* pdf, 
    SEVisibilityTester* visibility) const
{
    SEVector3f ns;
    SEVector3f ps = ShapeSet->Sample(p, ls, &ns);
    *wi = ps - p;
    wi->Normalize();
    *pdf = ShapeSet->GetPDF(p, *wi);
    visibility->SetSegment(p, pEpsilon, ps, 1e-3f, time);
    SESpectrum Ls = L(ps, ns, -*wi);

    return Ls;
}
//----------------------------------------------------------------------------
SESpectrum SEBNDiffuseAreaLight::Sample_L(const SEBNScene*, 
    const SEBNLightSample& ls, float u1, float u2, float time, SERay3f* ray, 
    SEVector3f* Ns, float* pdf) const
{
    SEVector3f org, dir;
    org = ShapeSet->Sample(ls, Ns);
    SEMonteCarlo::UniformSampleSphere(u1, u2, dir);

    if( dir.Dot(*Ns) < 0.0f )
    {
        dir *= -1.0f;
    }

    *ray = SERay3f(org, dir, 1e-3f, SEMathf::MAX_REAL, time);
    *pdf = ShapeSet->GetPDF(org) * SEMathf::INV_TWO_PI;
    SESpectrum Ls = L(org, *Ns, dir);

    return Ls;
}
//----------------------------------------------------------------------------