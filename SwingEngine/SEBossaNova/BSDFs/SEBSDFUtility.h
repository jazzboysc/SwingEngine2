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

#ifndef Swing_BSDFUtility_H
#define Swing_BSDFUtility_H

#include "SEBossaNovaLIB.h"
#include "SEVector3.h"
#include "SESpectrum.h"
#include "SEMath.h"

namespace Swing
{

//----------------------------------------------------------------------------
// BSDF inline functions.
// Calculations are done in local tangent space:
// S(1, 0, 0) 
// T(0, 1, 0)
// N(0, 0, 1)
//----------------------------------------------------------------------------
inline float SE_CosTheta(const SEVector3f& w);
inline float SE_AbsCosTheta(const SEVector3f& w);
inline float SE_SinTheta2(const SEVector3f& w);
inline float SE_SinTheta(const SEVector3f& w);
inline float SE_CosPhi(const SEVector3f& w);
inline float SE_SinPhi(const SEVector3f& w);
inline bool SE_SameHemisphere(const SEVector3f& w, const SEVector3f& wp);
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Fresnel functions.
//----------------------------------------------------------------------------
SE_BOSSA_NOVA_API SESpectrum SE_FresnelDielectric(float cosi, float cost, 
    const SESpectrum& etai, const SESpectrum& etat);
SE_BOSSA_NOVA_API SESpectrum SE_FresnelConductor(float cosi, 
    const SESpectrum& eta, const SESpectrum& k);
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
#define SE_BSDF_ALLOCATE(arena, Type) new (arena.Alloc(sizeof(Type))) Type
//----------------------------------------------------------------------------

#include "SEBSDFUtility.inl"

}

#endif