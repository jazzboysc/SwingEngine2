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
#include "SEBNScene.h"
#include "SEBNLight.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEBNScene::SEBNScene(SEBNPrimitive* accel, 
    const std::vector<SEBNLight*>& lights, SEBNVolumeRegion* volumeRegion)
{
    Lights = lights;
    Aggregate = accel;
    VolumeRegion = volumeRegion;
    Bound = Aggregate->GetWorldBound();
    if( VolumeRegion )
    {
        Bound = Union(Bound, VolumeRegion->GetWorldBound());
    }
}
//----------------------------------------------------------------------------
SEBNScene::~SEBNScene()
{
    SE_DELETE Aggregate;
    SE_DELETE VolumeRegion;

    for( int i = 0; i < (int)Lights.size(); ++i )
    {
        SE_DELETE Lights[i];
    }
}
//----------------------------------------------------------------------------
const SEAxisAlignedBox3f& SEBNScene::GetWorldBound() const
{
    return Bound;
}
//----------------------------------------------------------------------------
bool SEBNScene::Intersect(const SERay3f& ray, SEBNIntersection* isect) const
{
    bool hit = Aggregate->Intersect(ray, isect);

    return hit;
}
//----------------------------------------------------------------------------
bool SEBNScene::IntersectP(const SERay3f& ray) const
{
    bool hit = Aggregate->IntersectP(ray);
    
    return hit;
}
//----------------------------------------------------------------------------