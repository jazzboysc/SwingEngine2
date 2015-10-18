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
#include "SEDifferentialGeometry.h"
#include "SEMath.h"
#include "SELinearSystem.h"
#include "SEBNShape.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDifferentialGeometry::SEDifferentialGeometry()
{ 
    u = v = dudx = dvdx = dudy = dvdy = 0.0f;
    shape = 0; 
}
//----------------------------------------------------------------------------
SEDifferentialGeometry::SEDifferentialGeometry(const SEVector3f& P, 
    const SEVector3f& DPDU, const SEVector3f& DPDV, const SEVector3f& DNDU, 
    const SEVector3f& DNDV, float uu, float vv, const SEBNShape* sh)
    :
    p(P), dpdu(DPDU), dpdv(DPDV), dndu(DNDU), dndv(DNDV)
{
    // Initialize SEDifferentialGeometry from parameters.
    nn = dpdu.UnitCross(dpdv);
    u = uu;
    v = vv;
    shape = sh;
    dudx = dvdx = dudy = dvdy = 0.0f;

    // Adjust normal based on orientation.
    if( shape && shape->ReverseOrientation )
    {
        nn *= -1.0f;
    }
}
//----------------------------------------------------------------------------
void SEDifferentialGeometry::ComputeDifferentials(
    const SERayDifferential& ray) const
{
    if( ray.HasDifferentials )
    {
        // Estimate screen space change in geometry surface point and uv 
        // values.(dpdx, dpdy, dudx, dvdx, dudy, dvdy)

        // Compute auxiliary intersection points with plane.

        float d = -nn.Dot(p);

        float tx = -(nn.Dot(ray.RxOrigin) + d) / nn.Dot(ray.RxDirection);
        if( isnan(tx) )
        {
            dudx = dvdx = 0.0f;
            dudy = dvdy = 0.0f;
            dpdx = dpdy = SEVector3f::ZERO;

            return;
        }

        float ty = -(nn.Dot(ray.RyOrigin) + d) / nn.Dot(ray.RyDirection);
        if( isnan(ty) )
        {
            dudx = dvdx = 0.0f;
            dudy = dvdy = 0.0f;
            dpdx = dpdy = SEVector3f::ZERO;

            return;
        }

        SEVector3f px = ray.RxOrigin + tx*ray.RxDirection;
        SEVector3f py = ray.RyOrigin + ty*ray.RyDirection;
        dpdx = px - p;
        dpdy = py - p;

        // Compute uv offsets at auxiliary points.

        // Initialize matrices for offset computation.
        float A[2][2], Bx[2], By[2];
        int axes[2];
        if( SEMathf::FAbs(nn.X) > SEMathf::FAbs(nn.Y) && 
            SEMathf::FAbs(nn.X) > SEMathf::FAbs(nn.Z) )
        {
            axes[0] = 1;
            axes[1] = 2;
        }
        else if( SEMathf::FAbs(nn.Y) > SEMathf::FAbs(nn.Z) )
        {
            axes[0] = 0;
            axes[1] = 2;
        }
        else
        {
            axes[0] = 0;
            axes[1] = 1;
        }

        // Initialize matrices for chosen projection plane.
        A[0][0] = dpdu[axes[0]];
        A[0][1] = dpdv[axes[0]];
        A[1][0] = dpdu[axes[1]];
        A[1][1] = dpdv[axes[1]];
        Bx[0] = px[axes[0]] - p[axes[0]];
        Bx[1] = px[axes[1]] - p[axes[1]];
        By[0] = py[axes[0]] - p[axes[0]];
        By[1] = py[axes[1]] - p[axes[1]];

        // Solve the linear systems.
        SELinearSystem linearSystem(1e-10f);
        if( !linearSystem.Solve2x2(A, Bx, &dudx, &dvdx) )
        {
            dudx = 0.0f;
            dvdx = 0.0f;
        }
        if( !linearSystem.Solve2x2(A, By, &dudy, &dvdy) )
        {
            dudy = 0.0f;
            dvdy = 0.0f;
        }
    }
    else
    {
        dudx = dvdx = 0.0f;
        dudy = dvdy = 0.0f;
        dpdx = dpdy = SEVector3f::ZERO;
    }
}
//----------------------------------------------------------------------------