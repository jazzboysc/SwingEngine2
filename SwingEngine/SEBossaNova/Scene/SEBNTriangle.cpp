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
#include "SEBNTriangle.h"
#include "SEMonteCarlo.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SEBNTriangle, SEBNShape);

//----------------------------------------------------------------------------
SEBNTriangle::SEBNTriangle(const SETransformation* objectToWorld, 
    bool reverseOrientation, SEBNTriangleMesh* mesh, int n)
    : 
    SEBNShape(objectToWorld, reverseOrientation)
{
    SE_ASSERT( objectToWorld && mesh );

    Mesh = mesh;
    V = &mesh->SEIndexBuffer[3 * n];
}
//----------------------------------------------------------------------------
SEAxisAlignedBox3f SEBNTriangle::GetObjectBound() const
{
    const SEVector3f& p1 = Mesh->P[V[0]];
    const SEVector3f& p2 = Mesh->P[V[1]];
    const SEVector3f& p3 = Mesh->P[V[2]];
    
    SEVector3f tempP;
    ObjectToWorld.ApplyInverseToPoint(p1, tempP);
    SEAxisAlignedBox3f resBox(tempP);

    ObjectToWorld.ApplyInverseToPoint(p2, tempP);
    resBox = Union(resBox, tempP);

    ObjectToWorld.ApplyInverseToPoint(p3, tempP);
    resBox = Union(resBox, tempP);

    return resBox;
}
//----------------------------------------------------------------------------
SEAxisAlignedBox3f SEBNTriangle::GetWorldBound() const
{
    const SEVector3f& p1 = Mesh->P[V[0]];
    const SEVector3f& p2 = Mesh->P[V[1]];
    const SEVector3f& p3 = Mesh->P[V[2]];

    SEAxisAlignedBox3f resBox(p1, p2);
    resBox = Union(resBox, p3);

    return resBox;
}
//----------------------------------------------------------------------------
bool SEBNTriangle::Intersect(const SERay3f& ray, float* tHit, 
    float* rayEpsilon, SEDifferentialGeometry* dg) const
{
    SE_ASSERT( tHit && rayEpsilon && dg );

    // Get triangle vertices.
    const SEVector3f& p1 = Mesh->P[V[0]];
    const SEVector3f& p2 = Mesh->P[V[1]];
    const SEVector3f& p3 = Mesh->P[V[2]];

    SEVector3f e1 = p2 - p1;
    SEVector3f e2 = p3 - p1;
    SEVector3f s1 = ray.Direction.Cross(e2);
    float divisor = s1.Dot(e1);
    
    if( divisor == 0.0f )
    {
        return false;
    }
    float invDivisor = 1.0f / divisor;

    // Compute first barycentric coordinate.
    SEVector3f d = ray.Origin - p1;
    float b1 = d.Dot(s1) * invDivisor;
    if( b1 < -SEMathf::ZERO_TOLERANCE || b1 > 1.0f )
    {
        return false;
    }
    if( b1 < 0.0f )
    {
        b1 = 0.0f;
    }

    // Compute second barycentric coordinate.
    SEVector3f s2 = d.Cross(e1);
    float b2 = ray.Direction.Dot(s2) * invDivisor;
    if( b2 < -SEMathf::ZERO_TOLERANCE || b1 + b2 > 1.0f )
    {
        return false;
    }
    if( b2 < 0.0f )
    {
        b2 = 0.0f;
    }

    // Compute t to intersection point.
    float t = e2.Dot(s2) * invDivisor;
    if( t < ray.MinT || t > ray.MaxT )
    {
        return false;
    }

    // Compute triangle partial derivatives.
    SEVector3f dpdu, dpdv;
    float uvs[3][2];
    GetUVs(uvs);

    // Compute deltas for triangle partial derivatives.
    float du1 = uvs[0][0] - uvs[2][0];
    float du2 = uvs[1][0] - uvs[2][0];
    float dv1 = uvs[0][1] - uvs[2][1];
    float dv2 = uvs[1][1] - uvs[2][1];
    SEVector3f dp1 = p1 - p3, dp2 = p2 - p3;
    float determinant = du1 * dv2 - dv1 * du2;
    if( determinant == 0.0f )
    {
        SE_ASSERT( false );

        // Handle zero determinant for triangle partial derivative matrix.
        SEVector3f normal = e1.UnitCross(e2);
        SEVector3f::GetOrthonormalBasis(dpdu, dpdv, normal, true);
    }
    else
    {
        float invdet = 1.0f / determinant;
        dpdu = ( dv2 * dp1 - dv1 * dp2) * invdet;
        dpdv = (-du2 * dp1 + du1 * dp2) * invdet;
    }

    // Interpolate uv triangle parametric coordinates.
    float b0 = 1 - b1 - b2;
    float tu = b0*uvs[0][0] + b1*uvs[1][0] + b2*uvs[2][0];
    float tv = b0*uvs[0][1] + b1*uvs[1][1] + b2*uvs[2][1];

    // Test intersection against alpha texture, if present.
    if( ray.Depth != -1 )
    {
        if( Mesh->AlphaTexture )
        {
            SEDifferentialGeometry dgLocal(ray(t), dpdu, dpdv,
                SEVector3f::ZERO, SEVector3f::ZERO, tu, tv, this);

            if( Mesh->AlphaTexture->Evaluate(dgLocal) == 0.0f )
            {
                return false;
            }
        }
    }

    // Fill in SEDifferentialGeometry object from triangle hit point.
    *dg = SEDifferentialGeometry(ray(t), dpdu, dpdv,
        SEVector3f::ZERO, SEVector3f::ZERO, tu, tv, this);

    // Store barycentric coordinates for later use.
    float* pUserData = (float*)&dg->UserData[0];
    pUserData[0] = b0;
    pUserData[1] = b1;
    pUserData[2] = b2;

    *tHit = t;
    *rayEpsilon = 1e-3f * *tHit;

    return true;
}
//----------------------------------------------------------------------------
bool SEBNTriangle::IntersectP(const SERay3f& ray) const
{
    // Get triangle vertices.
    const SEVector3f& p1 = Mesh->P[V[0]];
    const SEVector3f& p2 = Mesh->P[V[1]];
    const SEVector3f& p3 = Mesh->P[V[2]];

    SEVector3f e1 = p2 - p1;
    SEVector3f e2 = p3 - p1;
    SEVector3f s1 = ray.Direction.Cross(e2);
    float divisor = s1.Dot(e1);
    
    if( divisor == 0.0f )
    {
        return false;
    }
    float invDivisor = 1.0f / divisor;

    // Compute first barycentric coordinate.
    SEVector3f d = ray.Origin - p1;
    float b1 = d.Dot(s1) * invDivisor;
    if( b1 <  -SEMathf::ZERO_TOLERANCE || b1 > 1.0f )
    {
        return false;
    }
    if( b1 < 0.0f )
    {
        b1 = 0.0f;
    }

    // Compute second barycentric coordinate.
    SEVector3f s2 = d.Cross(e1);
    float b2 = ray.Direction.Dot(s2) * invDivisor;
    if( b2 <  -SEMathf::ZERO_TOLERANCE || b1 + b2 > 1.0f )
    {
        return false;
    }
    if( b2 < 0.0f )
    {
        b2 = 0.0f;
    }

    // Compute t to intersection point.
    float t = e2.Dot(s2) * invDivisor;
    if( t < ray.MinT || t > ray.MaxT )
    {
        return false;
    }

    // Test shadow ray intersection against alpha texture, if present.
    if( ray.Depth != -1 && Mesh->AlphaTexture )
    {
        // Compute triangle partial derivatives.
        SEVector3f dpdu, dpdv;
        float uvs[3][2];
        GetUVs(uvs);

        // Compute deltas for triangle partial derivatives.
        float du1 = uvs[0][0] - uvs[2][0];
        float du2 = uvs[1][0] - uvs[2][0];
        float dv1 = uvs[0][1] - uvs[2][1];
        float dv2 = uvs[1][1] - uvs[2][1];
        SEVector3f dp1 = p1 - p3, dp2 = p2 - p3;
        float determinant = du1 * dv2 - dv1 * du2;
        if( determinant == 0.0f )
        {
            SE_ASSERT( false );

            // Handle zero determinant for triangle partial derivative matrix.
            SEVector3f normal = e1.UnitCross(e2);
            SEVector3f::GetOrthonormalBasis(dpdu, dpdv, normal, true);
        }
        else
        {
            float invdet = 1.0f / determinant;
            dpdu = ( dv2 * dp1 - dv1 * dp2) * invdet;
            dpdv = (-du2 * dp1 + du1 * dp2) * invdet;
        }

        // Interpolate uv triangle parametric coordinates.
        float b0 = 1 - b1 - b2;
        float tu = b0*uvs[0][0] + b1*uvs[1][0] + b2*uvs[2][0];
        float tv = b0*uvs[0][1] + b1*uvs[1][1] + b2*uvs[2][1];

        SEDifferentialGeometry dgLocal(ray(t), dpdu, dpdv,
            SEVector3f::ZERO, SEVector3f::ZERO, tu, tv, this);

        if( Mesh->AlphaTexture->Evaluate(dgLocal) == 0.0f )
        {
            return false;
        }
    }

    return true;
}
//----------------------------------------------------------------------------
float SEBNTriangle::GetArea() const
{
    // Get triangle vertices.
    const SEVector3f& p1 = Mesh->P[V[0]];
    const SEVector3f& p2 = Mesh->P[V[1]];
    const SEVector3f& p3 = Mesh->P[V[2]];

    SEVector3f e1 = p2 - p1;
    SEVector3f e2 = p3 - p1;
    SEVector3f normal = e1.Cross(e2);
    return 0.5f * normal.GetLength();
}
//----------------------------------------------------------------------------
void SEBNTriangle::GetShadingGeometry(const SETransformation& objectToWorld,
    const SEDifferentialGeometry& dg, SEDifferentialGeometry* dgShading) const
{
    if( !Mesh->N && !Mesh->S )
    {
        *dgShading = dg;
        return;
    }

    // Initialize shading geometry with n and s.

    // Get barycentric coordinates for the intersection point.
    float b[3];
    float* pUserData = (float*)&dg.UserData[0];
    b[0] = pUserData[0];
    b[1] = pUserData[1];
    b[2] = pUserData[2];

    // Use N and S to compute shading tangents for triangle, s and t.
    SEVector3f ns;
    SEVector3f ss, ts;
    if( Mesh->N )
    {
        SEVector3f objectSpaceN = 
            b[0]*Mesh->N[V[0]] + b[1]*Mesh->N[V[1]] + b[2]*Mesh->N[V[2]];

        objectToWorld.ApplyForwardToNormal(objectSpaceN, ns);
        ns.Normalize();
    }
    else
    {
        ns = dg.nn;
    }

    if( Mesh->S )
    {
        SEVector3f objectSpaceS =
            b[0]*Mesh->S[V[0]] + b[1]*Mesh->S[V[1]] + b[2]*Mesh->S[V[2]];

        objectToWorld.ApplyForwardToVector(objectSpaceS, ss);
        ss.Normalize();
    }
    else
    {
        ss = dg.dpdu;
        ss.Normalize();
    }
    
    ts = ss.Cross(ns);
    if( ts.GetSquaredLength() > 0.0f )
    {
        ts.Normalize();
        ss = ts.Cross(ns);
    }
    else
    {
        SEVector3f::GetOrthonormalBasis(ss, ts, ns, true);
    }

    // Compute dndu and dndv for triangle shading geometry.
    SEVector3f dndu, dndv;
    if( Mesh->N )
    {
        float uvs[3][2];
        GetUVs(uvs);

        // Compute deltas for triangle partial derivatives of normal.
        float du1 = uvs[0][0] - uvs[2][0];
        float du2 = uvs[1][0] - uvs[2][0];
        float dv1 = uvs[0][1] - uvs[2][1];
        float dv2 = uvs[1][1] - uvs[2][1];

        SEVector3f dn1 = Mesh->N[V[0]] - Mesh->N[V[2]];
        SEVector3f dn2 = Mesh->N[V[1]] - Mesh->N[V[2]];
        float determinant = du1 * dv2 - dv1 * du2;

        if( determinant == 0.0f )
        {
            dndu = dndv = SEVector3f::ZERO;
        }
        else
        {
            float invdet = 1.0f / determinant;
            dndu = ( dv2 * dn1 - dv1 * dn2) * invdet;
            dndv = (-du2 * dn1 + du1 * dn2) * invdet;
        }
    }
    else
    {
        dndu = dndv = SEVector3f::ZERO;
    }

    // Get world space triangle partial derivatives of normal.
    SEVector3f worldSpaceDNDU, worldSpaceDNDV;
    objectToWorld.ApplyForwardToNormal(dndu, worldSpaceDNDU);
    objectToWorld.ApplyForwardToNormal(dndv, worldSpaceDNDV);

    *dgShading = SEDifferentialGeometry(dg.p, ss, ts,
        worldSpaceDNDU, worldSpaceDNDV, dg.u, dg.v, dg.shape);

    dgShading->dudx = dg.dudx;  dgShading->dvdx = dg.dvdx;
    dgShading->dudy = dg.dudy;  dgShading->dvdy = dg.dvdy;
    dgShading->dpdx = dg.dpdx;  dgShading->dpdy = dg.dpdy;
}
//----------------------------------------------------------------------------
SEVector3f SEBNTriangle::Sample(float u1, float u2, SEVector3f* Ns) const
{
    SE_ASSERT( Ns );

    // Get triangle vertices.
    const SEVector3f& p1 = Mesh->P[V[0]];
    const SEVector3f& p2 = Mesh->P[V[1]];
    const SEVector3f& p3 = Mesh->P[V[2]];

    // Generate barycentric coordinates and the sample position.
    float b1, b2;
    SEMonteCarlo::UniformSampleTriangle(u1, u2, &b1, &b2);
    SEVector3f p = b1*p1 + b2*p2 + (1.0f - b1 - b2)*p3;

    SEVector3f e1 = p2 - p1;
    SEVector3f e2 = p3 - p1;
    *Ns = e1.UnitCross(e2);
    if( ReverseOrientation )
    {
        *Ns *= -1.0f;
    }

    return p;
}
//----------------------------------------------------------------------------
void SEBNTriangle::GetUVs(float uv[3][2]) const
{
    if( Mesh->UV )
    {
        uv[0][0] = Mesh->UV[2*V[0]    ];
        uv[0][1] = Mesh->UV[2*V[0] + 1];
        uv[1][0] = Mesh->UV[2*V[1]    ];
        uv[1][1] = Mesh->UV[2*V[1] + 1];
        uv[2][0] = Mesh->UV[2*V[2]    ];
        uv[2][1] = Mesh->UV[2*V[2] + 1];
    }
    else
    {
        uv[0][0] = 0.0f; uv[0][1] = 0.0f;
        uv[1][0] = 1.0f; uv[1][1] = 0.0f;
        uv[2][0] = 1.0f; uv[2][1] = 1.0f;
    }
}
//----------------------------------------------------------------------------