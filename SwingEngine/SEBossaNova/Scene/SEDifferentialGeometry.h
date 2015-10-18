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

#ifndef Swing_DifferentialGeometry_H
#define Swing_DifferentialGeometry_H

#include "SEBossaNovaLIB.h"
#include "SEVector3.h"
#include "SERayDifferential.h"

namespace Swing
{

class SEBNShape;
//----------------------------------------------------------------------------
// Description:
// Date:20130421
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEDifferentialGeometry
{
public:
    SEDifferentialGeometry();
    SEDifferentialGeometry(const SEVector3f& P, const SEVector3f& DPDU,
        const SEVector3f& DPDV, const SEVector3f& DNDU, 
        const SEVector3f& DNDV, float uu, float vv, const SEBNShape* sh);

    void ComputeDifferentials(const SERayDifferential& ray) const;

public:
    // CAUTION:
    // Position, normal and partial derivatives are in world space.

    SEVector3f p;
    SEVector3f nn;
    float u, v;
    const SEBNShape* shape;
    SEVector3f dpdu, dpdv;
    SEVector3f dndu, dndv;

    // Computed by function ComputeDifferentials.
    mutable SEVector3f dpdx, dpdy;
    mutable float dudx, dvdx, dudy, dvdy;

    // User specified data buffer.
    char UserData[16];
};

}

#endif