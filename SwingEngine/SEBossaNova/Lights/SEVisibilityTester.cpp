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
#include "SEVisibilityTester.h"
#include "SEMath.h"

using namespace Swing;

//----------------------------------------------------------------------------
void SEVisibilityTester::SetSegment(const SEVector3f& p1, float eps1, 
    const SEVector3f &p2, float eps2, float time)
{
    SEVector3f dir = p2 - p1;
    float dist = dir.GetLength();
    R = SERay3f(p1, dir / dist, eps1, dist * (1.0f - eps2), time);

    // TODO:
    //SE_ASSERT( !R.HasNaNs() );
}
//----------------------------------------------------------------------------
void SEVisibilityTester::SetRay(const SEVector3f& p, float eps, 
    const SEVector3f& w, float time)
{
    R = SERay3f(p, w, eps, SEMathf::MAX_REAL, time);

    // TODO:
    //SE_ASSERT( !R.HasNaNs() );
}
//----------------------------------------------------------------------------
bool SEVisibilityTester::IsUnoccluded(const SEBNScene* scene) const
{
    return !scene->IntersectP(R);
}
//----------------------------------------------------------------------------
SESpectrum SEVisibilityTester::Transmittance(const SEBNScene* scene, 
    const SEBNRenderer* renderer, const SEBNSample* sample, 
    SERandomNumberGenerator& rng, SEMemoryArena& arena) const
{
    return renderer->Transmittance(scene, SERayDifferential(R), sample, rng, 
        arena);
}
//----------------------------------------------------------------------------