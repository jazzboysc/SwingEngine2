// Swing Engine Version 2 Source Code 
// Most of techniques in the engine are mainly based on David Eberly's
// Wild Magic 4 and 5 open-source game engine. The author of Swing Engine 
// learned a lot from Eberly's experience of architecture and algorithm.
// Several subsystems are totally new, and others are reorganized or
// reimplimented based on Wild Magic's subsystems.
// Copyright (c) 2007-2011
//
// David Eberly's permission:
// Geometric Tools, LLC
// Copyright (c) 1998-2010
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt

#include "SECorePCH.h"
#include "SEAxisAlignedBox3.h"
#include "SEMath.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEAxisAlignedBox3f::SEAxisAlignedBox3f()
{
    Min[0] = SEMathf::MAX_REAL;
    Min[1] = SEMathf::MAX_REAL;
    Min[2] = SEMathf::MAX_REAL;
    Max[0] = -SEMathf::MAX_REAL;
    Max[1] = -SEMathf::MAX_REAL;
    Max[2] = -SEMathf::MAX_REAL;
}
//----------------------------------------------------------------------------
SEAxisAlignedBox3f::SEAxisAlignedBox3f(float fXMin, float fXMax, float fYMin,
    float fYMax, float fZMin, float fZMax)
{
    Min[0] = fXMin;
    Max[0] = fXMax;
    Min[1] = fYMin;
    Max[1] = fYMax;
    Min[2] = fZMin;
    Max[2] = fZMax;
}
//----------------------------------------------------------------------------
SEAxisAlignedBox3f::SEAxisAlignedBox3f(const SEVector3f& rP1, 
    const SEVector3f& rP2)
{
    Min[0] = SE_MIN(rP1.X, rP2.X);
    Min[1] = SE_MIN(rP1.Y, rP2.Y);
    Min[2] = SE_MIN(rP1.Z, rP2.Z);
    Max[0] = SE_MAX(rP1.X, rP2.X);
    Max[1] = SE_MAX(rP1.Y, rP2.Y);
    Max[2] = SE_MAX(rP1.Z, rP2.Z);
}
//----------------------------------------------------------------------------
SEAxisAlignedBox3f::SEAxisAlignedBox3f(const SEVector3f& rP)
{
    *this = rP;
}
//----------------------------------------------------------------------------
SEAxisAlignedBox3f& SEAxisAlignedBox3f::operator=(const SEVector3f& rP)
{
    Min[0] = rP.X;
    Min[1] = rP.Y;
    Min[2] = rP.Z;
    Max[0] = rP.X;
    Max[1] = rP.Y;
    Max[2] = rP.Z;

    return *this;
}
//----------------------------------------------------------------------------
bool SEAxisAlignedBox3f::operator==(const SEAxisAlignedBox3f& rBox) const
{
    if( Min[0] == rBox.Min[0] &&
        Min[1] == rBox.Min[1] &&
        Min[2] == rBox.Min[2] &&
        Max[0] == rBox.Max[0] &&
        Max[1] == rBox.Max[1] &&
        Max[2] == rBox.Max[2] )
    {
        return true;
    }

    return false;
}
//----------------------------------------------------------------------------
bool SEAxisAlignedBox3f::operator!=(const SEAxisAlignedBox3f& rBox) const
{
    if( Min[0] != rBox.Min[0] ||
        Min[1] != rBox.Min[1] ||
        Min[2] != rBox.Min[2] ||
        Max[0] != rBox.Max[0] ||
        Max[1] != rBox.Max[1] ||
        Max[2] != rBox.Max[2] )
    {
        return true;
    }

    return false;
}
//----------------------------------------------------------------------------
bool SEAxisAlignedBox3f::HasXOverlap(const SEAxisAlignedBox3f& rBox) const
{
    return Max[0] >= rBox.Min[0] && Min[0] <= rBox.Max[0];
}
//----------------------------------------------------------------------------
bool SEAxisAlignedBox3f::HasYOverlap(const SEAxisAlignedBox3f& rBox) const
{
    return Max[1] >= rBox.Min[1] && Min[1] <= rBox.Max[1];
}
//----------------------------------------------------------------------------
bool SEAxisAlignedBox3f::HasZOverlap(const SEAxisAlignedBox3f& rBox) const
{
    return Max[2] >= rBox.Min[2] && Min[2] <= rBox.Max[2];
}
//----------------------------------------------------------------------------
bool SEAxisAlignedBox3f::TestIntersection(const SEAxisAlignedBox3f& rBox) 
    const
{
    for( int i = 0; i < 3; i++ )
    {
        if( Max[i] < rBox.Min[i] || Min[i] > rBox.Max[i] )
        {
            return false;
        }
    }

    return true;
}
//----------------------------------------------------------------------------
bool SEAxisAlignedBox3f::FindIntersection(const SEAxisAlignedBox3f& rBox,
    SEAxisAlignedBox3f& rIntr) const
{
    int i;
    for( i = 0; i < 3; i++ )
    {
        if( Max[i] < rBox.Min[i] || Min[i] > rBox.Max[i] )
        {
            return false;
        }
    }

    for( i = 0; i < 3; i++ )
    {
        if( Max[i] <= rBox.Max[i] )
        {
            rIntr.Max[i] = Max[i];
        }
        else
        {
            rIntr.Max[i] = rBox.Max[i];
        }

        if( Min[i] <= rBox.Min[i] )
        {
            rIntr.Min[i] = rBox.Min[i];
        }
        else
        {
            rIntr.Min[i] = Min[i];
        }
    }

    return true;
}
//----------------------------------------------------------------------------
bool SEAxisAlignedBox3f::FindIntersection(const SERay3f& rRay, float* fHitT0,
    float* fHitT1) const
{
    float fT0 = rRay.MinT;
    float fT1 = rRay.MaxT;

    for( int i = 0; i < 3; ++i )
    {
        // Update interval for ith bounding box slab.
        float fInvRayDir = 1.0f / rRay.Direction[i];
        float fTNear = (Min[i] - rRay.Origin[i]) * fInvRayDir;
        float fTFar  = (Max[i] - rRay.Origin[i]) * fInvRayDir;

        // Update parametric interval from slab intersection Ts.

        if( fTNear > fTFar )
        {
            // Make sure fTNear <= fTFar.
            float fTemp = fTNear;
            fTNear = fTFar;
            fTFar = fTemp;
        }

        fT0 = fTNear > fT0 ? fTNear : fT0;
        fT1 = fTFar  < fT1 ? fTFar  : fT1;

        if( fT0 > fT1 )
        {
            return false;
        }
    }

    if( fHitT0 )
    {
        *fHitT0 = fT0;
    }

    if( fHitT1 )
    {
        *fHitT1 = fT1;
    }

    return true;
}
//----------------------------------------------------------------------------
bool SEAxisAlignedBox3f::IsInside(const SEVector3f& rP) const
{
    for( int i = 0; i < 3; ++i )
    {
        if( rP[i] < Min[i] || rP[i] > Max[i] )
        {
            return false;
        }
    }

    return true;
}
//----------------------------------------------------------------------------
void SEAxisAlignedBox3f::Expand(float fDelta)
{
    Min[0] -= fDelta;
    Min[1] -= fDelta;
    Min[2] -= fDelta;
    Max[0] += fDelta;
    Max[1] += fDelta;
    Max[2] += fDelta;
}
//----------------------------------------------------------------------------
float SEAxisAlignedBox3f::GetSurfaceArea() const
{
    float dx = Max[0] - Min[0];
    float dy = Max[1] - Min[1];
    float dz = Max[2] - Min[2];
    SE_ASSERT( dx >= 0.0f && dy >= 0.0f && dz >= 0.0f );

    return 2.0f * (dx*dy + dx*dz + dy*dz);
}
//----------------------------------------------------------------------------
float SEAxisAlignedBox3f::GetVolume() const
{
    float dx = Max[0] - Min[0];
    float dy = Max[1] - Min[1];
    float dz = Max[2] - Min[2];
    SE_ASSERT( dx >= 0.0f && dy >= 0.0f && dz >= 0.0f );

    return dx*dy*dz;
}
//----------------------------------------------------------------------------
int SEAxisAlignedBox3f::GetMaximumExtent() const
{
    float dx = Max[0] - Min[0];
    float dy = Max[1] - Min[1];
    float dz = Max[2] - Min[2];
    SE_ASSERT( dx >= 0.0f && dy >= 0.0f && dz >= 0.0f );

    if( dx > dy && dx > dz)
    {
        return 0;
    }
    else if( dy > dz)
    {
        return 1;
    }
    else
    {
        return 2;
    }
}
//----------------------------------------------------------------------------
SEVector3f SEAxisAlignedBox3f::GetDelta() const
{
    SEVector3f tempRes;

    tempRes.X = Max[0] - Min[0];
    tempRes.Y = Max[1] - Min[1];
    tempRes.Z = Max[2] - Min[2];

    return tempRes;
}
//----------------------------------------------------------------------------
SEVector3f SEAxisAlignedBox3f::GetBoxCenter() const
{
    SEVector3f tempRes;

    tempRes.X = 0.5f*(Max[0] + Min[0]);
    tempRes.Y = 0.5f*(Max[1] + Min[1]);
    tempRes.Z = 0.5f*(Max[2] + Min[2]);

    return tempRes;
}
//----------------------------------------------------------------------------
SEVector3f SEAxisAlignedBox3f::GetExtension() const
{
    SEVector3f tempRes;

    tempRes.X = 0.5f*(Max[0] - Min[0]);
    tempRes.Y = 0.5f*(Max[1] - Min[1]);
    tempRes.Z = 0.5f*(Max[2] - Min[2]);

    return tempRes;
}
//----------------------------------------------------------------------------
void SEAxisAlignedBox3f::Merge(const SEAxisAlignedBox3f& box)
{
    Min[0] = SE_MIN(Min[0], box.Min[0]);
    Min[1] = SE_MIN(Min[1], box.Min[1]);
    Min[2] = SE_MIN(Min[2], box.Min[2]);
    Max[0] = SE_MAX(Max[0], box.Max[0]);
    Max[1] = SE_MAX(Max[1], box.Max[1]);
    Max[2] = SE_MAX(Max[2], box.Max[2]);
}
//----------------------------------------------------------------------------
void SEAxisAlignedBox3f::ExpandToCube()
{
    SEVector3f center = GetBoxCenter();
    SEVector3f extension = GetExtension();
    float extMax = SE_MAX(SE_MAX(extension.X, extension.Y), extension.Z);
    Min[0] = center[0] - extMax;
    Min[1] = center[1] - extMax;
    Min[2] = center[2] - extMax;
    Max[0] = center[0] + extMax;
    Max[1] = center[1] + extMax;
    Max[2] = center[2] + extMax;
}
//----------------------------------------------------------------------------
SEAxisAlignedBox3f Swing::Union(const SEAxisAlignedBox3f& rBox, 
    const SEVector3f& rP)
{
    SEAxisAlignedBox3f Res = rBox;

    Res.Min[0] = SE_MIN(rBox.Min[0], rP.X);
    Res.Min[1] = SE_MIN(rBox.Min[1], rP.Y);
    Res.Min[2] = SE_MIN(rBox.Min[2], rP.Z);
    Res.Max[0] = SE_MAX(rBox.Max[0], rP.X);
    Res.Max[1] = SE_MAX(rBox.Max[1], rP.Y);
    Res.Max[2] = SE_MAX(rBox.Max[2], rP.Z);

    return Res;
}
//----------------------------------------------------------------------------
SEAxisAlignedBox3f Swing::Union(const SEAxisAlignedBox3f& rBox1, 
    const SEAxisAlignedBox3f& rBox2)
{
    SEAxisAlignedBox3f Res;

    Res.Min[0] = SE_MIN(rBox1.Min[0], rBox2.Min[0]);
    Res.Min[1] = SE_MIN(rBox1.Min[1], rBox2.Min[1]);
    Res.Min[2] = SE_MIN(rBox1.Min[2], rBox2.Min[2]);
    Res.Max[0] = SE_MAX(rBox1.Max[0], rBox2.Max[0]);
    Res.Max[1] = SE_MAX(rBox1.Max[1], rBox2.Max[1]);
    Res.Max[2] = SE_MAX(rBox1.Max[2], rBox2.Max[2]);

    return Res;
}
//----------------------------------------------------------------------------