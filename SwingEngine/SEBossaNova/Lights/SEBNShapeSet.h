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

#ifndef Swing_BNShapeSet_H
#define Swing_BNShapeSet_H

#include "SEBossaNovaLIB.h"
#include "SEBNShape.h"
#include "SEBNLightSample.h"

namespace Swing
{

class SEDistribution1D;
//----------------------------------------------------------------------------
// Description:
// Date:20130527
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEBNShapeSet
{
public:
    SEBNShapeSet(SEBNShape* shape);
    ~SEBNShapeSet();

    float GetArea() const;

    SEVector3f Sample(const SEVector3f& p, const SEBNLightSample& ls, 
        SEVector3f* Ns) const;
    float GetPDF(const SEVector3f& p, const SEVector3f& wi) const;

    SEVector3f Sample(const SEBNLightSample& ls, SEVector3f* Ns) const;
    float GetPDF(const SEVector3f& p) const;

private:
    std::vector<SEBNShapePtr> Shapes;
    float SumArea, InvSumArea;
    std::vector<float> Areas;
    SEDistribution1D* AreaDistribution;
};

}

#endif