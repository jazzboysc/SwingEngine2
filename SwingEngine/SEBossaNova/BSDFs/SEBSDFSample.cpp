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
#include "SEBSDFSample.h"

using namespace Swing;

//----------------------------------------------------------------------------
// SEBSDFSampleOffsets functions.
//----------------------------------------------------------------------------
SEBSDFSampleOffsets::SEBSDFSampleOffsets()
{
    SampleCount = 0;
    ComponentOffset = 0;
    DirOffset = 0;
}
//----------------------------------------------------------------------------
SEBSDFSampleOffsets::SEBSDFSampleOffsets(int count, SEBNSample* sample)
{
    SampleCount = count;
    ComponentOffset = sample->Add1DSamples(SampleCount);
    DirOffset = sample->Add2DSamples(SampleCount);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// SEBSDFSample functions.
//----------------------------------------------------------------------------
SEBSDFSample::SEBSDFSample(float up0, float up1, float ucomp)
{
    SE_ASSERT( up0 >= 0.0f && up0 < 1.0f );
    SE_ASSERT( up1 >= 0.0f && up1 < 1.0f );
    SE_ASSERT( ucomp >= 0.0f && ucomp < 1.0f );

    UDir[0] = up0;
    UDir[1] = up1;
    UComponent = ucomp;
}
//----------------------------------------------------------------------------
SEBSDFSample::SEBSDFSample(SERandomNumberGenerator& rng)
{
    UDir[0] = rng.RandomFloat();
    UDir[1] = rng.RandomFloat();
    UComponent = rng.RandomFloat();
}
//----------------------------------------------------------------------------
SEBSDFSample::SEBSDFSample(const SEBNSample* sample, 
   const SEBSDFSampleOffsets& offsets, SE_UInt32 n)
{
    SE_ASSERT( n < sample->N2D[offsets.DirOffset] );
    SE_ASSERT( n < sample->N1D[offsets.ComponentOffset] );

    UDir[0] = sample->TwoD[offsets.DirOffset][2*n   ];
    UDir[1] = sample->TwoD[offsets.DirOffset][2*n + 1];
    UComponent = sample->OneD[offsets.ComponentOffset][n];

    SE_ASSERT( UDir[0] >= 0.0f && UDir[0] < 1.0f );
    SE_ASSERT( UDir[1] >= 0.0f && UDir[1] < 1.0f );
    SE_ASSERT( UComponent >= 0.0f && UComponent < 1.0f );
}
//----------------------------------------------------------------------------
SEBSDFSample::SEBSDFSample()
{
    UDir[0] = 0.0f;
    UDir[1] = 0.0f;
    UComponent = 0.0f;
}
//----------------------------------------------------------------------------