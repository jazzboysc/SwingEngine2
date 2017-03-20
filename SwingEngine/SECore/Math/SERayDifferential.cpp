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

#include "SECorePCH.h"
#include "SERayDifferential.h"


using namespace Swing;

//----------------------------------------------------------------------------
SERayDifferential::SERayDifferential()
{
    HasDifferentials = false;
}
//----------------------------------------------------------------------------
SERayDifferential::SERayDifferential(const SEVector3f& rOrg, 
    const SEVector3f& rDir, float fStart, float fEnd, float fTime, int iDepth)
    : 
    SERay3f(rOrg, rDir, fStart, fEnd, fTime, iDepth)
{
    HasDifferentials = false;
}
//----------------------------------------------------------------------------
SERayDifferential::SERayDifferential(const SEVector3f& rOrg, 
    const SEVector3f& rDir, const SERay3f& rParent, float fStart, float fEnd)
    : 
    SERay3f(rOrg, rDir, fStart, fEnd, rParent.Time, rParent.Depth + 1)
{
    HasDifferentials = false;
}
//----------------------------------------------------------------------------
SERayDifferential::SERayDifferential(const SERay3f& rRay)
    :
    SERay3f(rRay)
{
    HasDifferentials = false;
}
//----------------------------------------------------------------------------
void SERayDifferential::ScaleDifferentials(float fS)
{
    RxOrigin = Origin + (RxOrigin - Origin)*fS;
    RyOrigin = Origin + (RyOrigin - Origin)*fS;
    RxDirection = Direction + (RxDirection - Direction)*fS;
    RyDirection = Direction + (RyDirection - Direction)*fS;
}
//----------------------------------------------------------------------------