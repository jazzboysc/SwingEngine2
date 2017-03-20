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
#include "SEBNShapeSet.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEBNShapeSet::SEBNShapeSet(SEBNShape* shape)
{
    SE_ASSERT( shape );

    std::vector<SEBNShapePtr> todo;
    todo.push_back(shape);

    while( todo.size() )
    {
        SEBNShapePtr sh = todo.back();
        todo.pop_back();

        if( sh->CanIntersect() )
        {
            Shapes.push_back(sh);
        }
        else
        {
            sh->Refine(todo);
        }
    }

    if( Shapes.size() > 64 )
    {
        // Too many shapes in area light geometry may be very inefficient.
        SE_ASSERT( false );
    }

    // Compute total area of shapes in ShapeSet and area CDF.
    SumArea = 0.0f;
    for( SE_UInt32 i = 0; i < Shapes.size(); ++i )
    {
        float a = Shapes[i]->GetArea();
        Areas.push_back(a);
        SumArea += a;
    }
    InvSumArea = 1.0f / SumArea;
    AreaDistribution = SE_NEW SEDistribution1D(&Areas[0], (int)Areas.size());
}
//----------------------------------------------------------------------------
SEBNShapeSet::~SEBNShapeSet()
{
    SE_DELETE AreaDistribution;
}
//----------------------------------------------------------------------------
float SEBNShapeSet::GetArea() const
{
    return SumArea;
}
//----------------------------------------------------------------------------
SEVector3f SEBNShapeSet::Sample(const SEVector3f& p, 
    const SEBNLightSample& ls, SEVector3f* Ns) const
{
    int sn = AreaDistribution->SampleDiscrete(ls.UComponent, 0);
    SEVector3f pt = Shapes[sn]->Sample(p, ls.UPos[0], ls.UPos[1], Ns);

    if( Shapes.size() == 1 )
    {
        return pt;
    }

    // Find closest intersection of ray with shapes in ShapeSet.
    SERay3f r(p, pt - p, 1e-3f, SEMathf::MAX_REAL);
    float rayEps, thit = 1.0f;
    bool anyHit = false;
    SEDifferentialGeometry dg;
    for( SE_UInt32 i = 0; i < Shapes.size(); ++i )
    {
        anyHit |= Shapes[i]->Intersect(r, &thit, &rayEps, &dg);
    }

    if( anyHit )
    {
        *Ns = dg.nn;
    }

    return r(thit);
}
//----------------------------------------------------------------------------
float SEBNShapeSet::GetPDF(const SEVector3f& p, const SEVector3f& wi) const
{
    float pdf = 0.0f;
    for( SE_UInt32 i = 0; i < Shapes.size(); ++i )
    {
        pdf += Areas[i] * Shapes[i]->GetPDF(p, wi);
    }

    return pdf * InvSumArea;
}
//----------------------------------------------------------------------------
SEVector3f SEBNShapeSet::Sample(const SEBNLightSample& ls, 
    SEVector3f* Ns) const
{
    int sn = AreaDistribution->SampleDiscrete(ls.UComponent, 0);

    return Shapes[sn]->Sample(ls.UPos[0], ls.UPos[1], Ns);
}
//----------------------------------------------------------------------------
float SEBNShapeSet::GetPDF(const SEVector3f& p) const
{
    float pdf = 0.0f;
    for( SE_UInt32 i = 0; i < Shapes.size(); ++i )
    {
        pdf += Areas[i] * Shapes[i]->GetPDF(p);
    }

    return pdf * InvSumArea;
}
//----------------------------------------------------------------------------