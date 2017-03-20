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

#include "SEBossaNovaPCH.h"
#include "SEBNMatteMaterial.h"
#include "SEMath.h"
#include "SEBSDFUtility.h"
#include "SELambertian.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEBNMatteMaterial::SEBNMatteMaterial(SEBNTexture<SESpectrum>* kd, 
    SEBNTexture<float>* sigma, SEBNTexture<float>* bump)
    :
    Kd(kd), 
    Sigma(sigma), 
    BumpMap(bump)
{
}
//----------------------------------------------------------------------------
SEBSDF* SEBNMatteMaterial::GetBSDF(const SEDifferentialGeometry& dgGeom,
    const SEDifferentialGeometry& dgShading, SEMemoryArena& arena) const
{
    // Allocate BSDF, possibly doing bump mapping with BumpMap.
    SEDifferentialGeometry dgs;
    if( BumpMap )
    {
        Bump(BumpMap, dgGeom, dgShading, &dgs);
    }
    else
    {
        dgs = dgShading;
    }
    SEBSDF* bsdf = SE_BSDF_ALLOCATE(arena, SEBSDF)(dgs, dgGeom.nn);

    // Evaluate textures for MatteMaterial material and allocate BRDF.
    SESpectrum r = Kd->Evaluate(dgs).Clamp();
    float sigma = SE_Clamp(Sigma->Evaluate(dgs), 0.0f, 90.0f);
    if( !r.IsBlack() )
    {
        if( sigma == 0.0f )
        {
            bsdf->AddComponent(SE_BSDF_ALLOCATE(arena, SELambertian)(r));
        }
        else
        {
            // TODO:
            //bsdf->AddComponent(SE_BSDF_ALLOCATE(arena, SEOrenNayar)(r, 
            //    sigma));
        }
    }

    return bsdf;
}
//----------------------------------------------------------------------------