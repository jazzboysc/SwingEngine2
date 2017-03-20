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
#include "SEBNMaterial.h"
#include "SEBNShape.h"
#include "SEMath.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SEBNMaterial, SEObject);

//----------------------------------------------------------------------------
SEBNMaterial::SEBNMaterial()
{
}
//----------------------------------------------------------------------------
SEBNMaterial::~SEBNMaterial()
{
}
//----------------------------------------------------------------------------
SEBSSRDF* SEBNMaterial::GetBSSRDF(const SEDifferentialGeometry&,
    const SEDifferentialGeometry&, SEMemoryArena&) const
{
    return 0;
}
//----------------------------------------------------------------------------
void SEBNMaterial::Bump(const SEBNTexture<float>* displacement,
    const SEDifferentialGeometry& dgGeom,
    const SEDifferentialGeometry& dgs,
    SEDifferentialGeometry* dgBump)
{
    // Compute offset positions and evaluate displacement texture.
    SEDifferentialGeometry dgEval = dgs;

    // Shift dgEval du in the u direction.
    float du = 0.5f * (fabsf(dgs.dudx) + fabsf(dgs.dudy));
    if( du == 0.0f )
    {
        du = 0.01f;
    }
    dgEval.p = dgs.p + du * dgs.dpdu;
    dgEval.u = dgs.u + du;
    dgEval.nn = dgs.dpdu.Cross(dgs.dpdv) + du*dgs.dndu;
    dgEval.nn.Normalize();

    float uDisplace = displacement->Evaluate(dgEval);

    // Shift dgEval dv in the v direction.
    float dv = 0.5f * (fabsf(dgs.dvdx) + fabsf(dgs.dvdy));
    if( dv == 0.0f )
    {
        dv = 0.01f;
    }
    dgEval.p = dgs.p + dv * dgs.dpdv;
    dgEval.u = dgs.u;
    dgEval.v = dgs.v + dv;
    dgEval.nn = dgs.dpdu.Cross(dgs.dpdv) + dv*dgs.dndv;
    dgEval.nn.Normalize();

    float vDisplace = displacement->Evaluate(dgEval);
    float displace = displacement->Evaluate(dgs);

    // Compute bump-mapped differential geometry.
    *dgBump = dgs;
    dgBump->dpdu = dgs.dpdu + (uDisplace - displace) / du * dgs.nn +
        displace * dgs.dndu;
    dgBump->dpdv = dgs.dpdv + (vDisplace - displace) / dv * dgs.nn +
        displace * dgs.dndv;
    dgBump->nn = dgBump->dpdu.UnitCross(dgBump->dpdv);

    if( dgs.shape->ReverseOrientation )
    {
        dgBump->nn *= -1.0f;
    }

    // Orient shading normal to match geometric normal.
    SEVector3f::Faceforward(dgBump->nn, dgGeom.nn, dgBump->nn);
}
//----------------------------------------------------------------------------