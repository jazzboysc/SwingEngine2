// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_LinearSystem_H
#define Swing_LinearSystem_H

#include "SEGeometricToolsLIB.h"
#include "SEPlatforms.h"
#include "SEMath.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20130422
//----------------------------------------------------------------------------
class SE_GEOMETRICTOOLS_API SELinearSystem
{
public:
    SELinearSystem(float fZeroTolerance = SEMathf::ZERO_TOLERANCE);
    ~SELinearSystem();

    bool Solve2x2(const float A[2][2], const float B[2], float* X0, 
        float* X1);

    // Tolerance for linear system solving.
    float ZeroTolerance;
};

}

#endif