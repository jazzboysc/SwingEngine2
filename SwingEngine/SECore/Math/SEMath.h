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

#ifndef Swing_Math_H
#define Swing_Math_H

#include "SEPlatforms.h"
#include "SESystem.h"
#include "SEMemory.h"

namespace Swing
{

template <class Real>
class SEMath
{
public:
    // Wrappers to hide implementations of functions.  The ACos and ASin
    // functions clamp the input argument to [-1,1] to avoid NaN issues
    // when the input is slightly larger than 1 or slightly smaller than -1.
    // Other functions have the potential for using a fast and approximate
    // algorithm rather than calling the standard math library functions.
    static Real ACos(Real fValue);
    static Real ASin(Real fValue);
    static Real ATan(Real fValue);
    static Real ATan2(Real fY, Real fX);
    static Real Ceil(Real fValue);
    static Real Cos(Real fValue);
    static Real Exp(Real fValue);
    static Real FAbs(Real fValue);
    static Real Floor(Real fValue);
    static Real FMod(Real fX, Real fY);
    static Real InvSqrt(Real fValue);
    static Real Log(Real fValue);
    static Real Log2(Real fValue);
    static Real Pow(Real fBase, Real fExponent);
    static Real Sin(Real fValue);
    static Real Sqr(Real fValue);
    static Real Sqrt(Real fValue);
    static Real Tan(Real fValue);

    // Return -1 if the input is negative, 0 if the input is zero, and +1
    // if the input is positive.
    static int Sign(int iValue);
    static Real Sign(Real fValue);

    // Generate a random number in [0,1).  The random number generator may
    // be seeded by a first call to UnitRandom with a positive seed.
    static Real UnitRandom(unsigned int uiSeed = 0);

    // Generate a random number in [-1,1).  The random number generator may
    // be seeded by a first call to SymmetricRandom with a positive seed.
    static Real SymmetricRandom(unsigned int uiSeed = 0);

    // Generate a random number in [min,max).  The random number generator may
    // be seeded by a first call to IntervalRandom with a positive seed.
    static Real IntervalRandom(Real fMin, Real fMax, unsigned int uiSeed = 0);

    // Fast evaluation of trigonometric and inverse trigonometric functions
    // using polynomial approximations.  The speed ups were measured on an
    // AMD 2800 (2.08 GHz) processor using Visual Studion .NET 2003 with a
    // release build.

    // The input must be in [0,pi/2].
    // max error sin0 = 1.7e-04, speed up = 4.0
    // max error sin1 = 1.9e-08, speed up = 2.8
    static Real FastSin0(Real fAngle);
    static Real FastSin1(Real fAngle);

    // The input must be in [0,pi/2]
    // max error cos0 = 1.2e-03, speed up = 4.5
    // max error cos1 = 6.5e-09, speed up = 2.8
    static Real FastCos0(Real fAngle);
    static Real FastCos1(Real fAngle);

    // The input must be in [0,pi/4].
    // max error tan0 = 8.1e-04, speed up = 5.6
    // max error tan1 = 1.9e-08, speed up = 3.4
    static Real FastTan0(Real fAngle);
    static Real FastTan1(Real fAngle);

    // The input must be in [0,1].
    // max error invsin0 = 6.8e-05, speed up = 7.5
    // max error invsin1 = 1.4e-07, speed up = 5.5
    static Real FastInvSin0(Real fValue);
    static Real FastInvSin1(Real fValue);

    // The input must be in [0,1].
    // max error invcos0 = 6.8e-05, speed up = 7.5
    // max error invcos1 = 1.4e-07, speed up = 5.7
    static Real FastInvCos0(Real fValue);
    static Real FastInvCos1(Real fValue);

    // The input must be in [-1,1]. 
    // max error invtan0 = 1.2e-05, speed up = 2.8
    // max error invtan1 = 2.3e-08, speed up = 1.8
    static Real FastInvTan0(Real fValue);
    static Real FastInvTan1(Real fValue);

    // A fast approximation to 1/sqrt.
    static Real FastInvSqrt(Real fValue);

    // common constants
    SE_CORE_API static const Real EPSILON;
    SE_CORE_API static const Real ZERO_TOLERANCE;
    SE_CORE_API static const Real MAX_REAL;
    SE_CORE_API static const Real PI;
    SE_CORE_API static const Real TWO_PI;
    SE_CORE_API static const Real HALF_PI;
    SE_CORE_API static const Real INV_PI;
    SE_CORE_API static const Real INV_TWO_PI;
    SE_CORE_API static const Real DEG_TO_RAD;
    SE_CORE_API static const Real RAD_TO_DEG;
};

#include "SEMath.inl"

//----------------------------------------------------------------------------
// Fast conversion from a IEEE 32-bit floating point number F in [0,1] to a
// a 32-bit integer I in [0,2^L-1].
//
//   fFloat = F
//   iLog = L
//   iInt = I
#define SE_SCALED_FLOAT_TO_INT(fFloat,iLog,iInt)\
{ \
    int iShift = 150 - iLog - ((*(int*)(&fFloat) >> 23) & 0xFF); \
    if( iShift < 24 ) \
    { \
        iInt = ((*(int*)(&fFloat) & 0x007FFFFF) | \
            0x00800000) >> iShift; \
        if( iInt == (1 << iLog) ) \
        { \
            iInt--; \
        } \
    } \
    else \
    { \
        iInt = 0; \
    } \
}
//----------------------------------------------------------------------------
#define SE_SQR(x)((x) * (x))
//----------------------------------------------------------------------------
#define SE_MAX(a,b)((a < b) ? (b) : (a))
//----------------------------------------------------------------------------
#define SE_MIN(a,b)((a < b) ? (a) : (b))
//----------------------------------------------------------------------------
inline float SE_Lerp(float t, float v1, float v2)
{
    return (1.0f - t)*v1 + t*v2;
}
//----------------------------------------------------------------------------
inline float SE_Clamp(float fValue, float fLow, float fHigh)
{
    if( fValue < fLow )
    {
        return fLow;
    }
    else if( fValue > fHigh )
    {
        return fHigh;
    }
    else 
    {
        return fValue;
    }
}
//----------------------------------------------------------------------------
inline int SE_Clamp(int iValue, int iLow, int iHigh)
{
    if( iValue < iLow ) 
    {
        return iLow;
    }
    else if( iValue > iHigh )
    {
        return iHigh;
    }
    else
    {
        return iValue;
    }
}
//----------------------------------------------------------------------------
inline int SE_FloorToInt(float fValue)
{
    return (int)floorf(fValue);
}
//----------------------------------------------------------------------------
inline int SE_CeilToInt(float fValue)
{
    return (int)ceilf(fValue);
}
//----------------------------------------------------------------------------
inline int SE_RoundToInt(float fValue)
{
    return SE_FloorToInt(fValue + 0.5f);
}
//----------------------------------------------------------------------------
inline float SE_Log2(float fValue)
{
    static float fInvLog2 = 1.0f / logf(2.0f);
    return logf(fValue) * fInvLog2;
}
//----------------------------------------------------------------------------
inline int SE_Log2FloorToInt(float fValue)
{
    return SE_FloorToInt(SE_Log2(fValue));
}
//----------------------------------------------------------------------------

typedef SEMath<float> SEMathf;
typedef SEMath<double> SEMathd;

}

#endif