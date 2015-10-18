// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SEGeometricToolsPCH.h"
#include "SELinearSystem.h"

using namespace Swing;

//----------------------------------------------------------------------------
SELinearSystem::SELinearSystem(float fZeroTolerance)
    :
    ZeroTolerance(fZeroTolerance)
{
}
//----------------------------------------------------------------------------
SELinearSystem::~SELinearSystem()
{
}
//----------------------------------------------------------------------------
bool SELinearSystem::Solve2x2(const float A[2][2], const float B[2], 
    float* X0, float* X1)
{
    float fDet = A[0][0]*A[1][1] - A[0][1]*A[1][0];
    if( SEMathf::FAbs(fDet) < ZeroTolerance )
    {
        return false;
    }

    float fInvDet = 1.0f / fDet;
    *X0 = (A[1][1]*B[0] - A[0][1]*B[1])*fInvDet;
    *X1 = (A[0][0]*B[1] - A[1][0]*B[0])*fInvDet;

    if( isnan(*X0) || isnan(*X1) )
    {
        return false;
    }

    return true;
}
//----------------------------------------------------------------------------