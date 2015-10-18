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

#ifndef Swing_BNVolumeRegion_H
#define Swing_BNVolumeRegion_H

#include "SEBossaNovaLIB.h"
#include "SEAxisAlignedBox3.h"
#include "SERay3.h"
#include "SESpectrum.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20130602
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEBNVolumeRegion
{
public:
    SEBNVolumeRegion();
    virtual ~SEBNVolumeRegion();

    virtual SEAxisAlignedBox3f GetWorldBound() const = 0;

    virtual bool IntersectP(const SERay3f& ray, float* t0, 
        float* t1) const = 0;

    virtual SESpectrum sigma_a(const SEVector3f&, const SEVector3f&,
        float time) const = 0;

    virtual SESpectrum sigma_s(const SEVector3f&, const SEVector3f&,
        float time) const = 0;

    virtual SESpectrum Lve(const SEVector3f&, const SEVector3f&,
        float time) const = 0;

    virtual float p(const SEVector3f&, const SEVector3f&, const SEVector3f&, 
        float time) const = 0;

    virtual SESpectrum sigma_t(const SEVector3f& p, const SEVector3f& wo, 
        float time) const;

    virtual SESpectrum tau(const SERay3f& ray, float step = 1.0f,
        float offset = 0.5f) const = 0;
};

}

#endif