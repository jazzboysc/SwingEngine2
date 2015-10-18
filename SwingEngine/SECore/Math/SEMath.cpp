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
#include "SEMath.h"

namespace Swing
{
template<> const float SEMath<float>::EPSILON = FLT_EPSILON;
template<> const float SEMath<float>::ZERO_TOLERANCE = 1e-06f;
template<> const float SEMath<float>::MAX_REAL = FLT_MAX;
template<> const float SEMath<float>::PI = (float)(4.0*atan(1.0));
template<> const float SEMath<float>::TWO_PI = 2.0f*SEMath<float>::PI;
template<> const float SEMath<float>::HALF_PI = 0.5f*SEMath<float>::PI;
template<> const float SEMath<float>::INV_PI = 1.0f/SEMath<float>::PI;
template<> const float SEMath<float>::INV_TWO_PI = 1.0f/SEMath<float>::TWO_PI;
template<> const float SEMath<float>::DEG_TO_RAD = SEMath<float>::PI/180.0f;
template<> const float SEMath<float>::RAD_TO_DEG = 180.0f/SEMath<float>::PI;

template<> const double SEMath<double>::EPSILON = DBL_EPSILON;
template<> const double SEMath<double>::ZERO_TOLERANCE = 1e-08;
template<> const double SEMath<double>::MAX_REAL = DBL_MAX;
template<> const double SEMath<double>::PI = 4.0*atan(1.0);
template<> const double SEMath<double>::TWO_PI = 2.0*SEMath<double>::PI;
template<> const double SEMath<double>::HALF_PI = 0.5*SEMath<double>::PI;
template<> const double SEMath<double>::INV_PI = 1.0/SEMath<double>::PI;
template<> const double SEMath<double>::INV_TWO_PI = 
    1.0/SEMath<double>::TWO_PI;
template<> const double SEMath<double>::DEG_TO_RAD = SEMath<double>::PI/180.0;
template<> const double SEMath<double>::RAD_TO_DEG = 180.0/SEMath<double>::PI;

//----------------------------------------------------------------------------
template <>
float SEMath<float>::FastInvSqrt(float fValue)
{
    float fHalf = 0.5f*fValue;
    int i  = *(int*)&fValue;
    i = 0x5f3759df - (i >> 1);
    fValue = *(float*)&i;
    fValue = fValue*(1.5f - fHalf*fValue*fValue);
    return fValue;
}
//----------------------------------------------------------------------------
template <>
double SEMath<double>::FastInvSqrt(double dValue)
{
    double dHalf = 0.5*dValue;
    SE_Int64 i  = *(SE_Int64*)&dValue;
#if defined(SE_USING_VC70) || defined(SE_USING_VC6)
    i = 0x5fe6ec85e7de30da - (i >> 1);
#else
    i = 0x5fe6ec85e7de30daLL - (i >> 1);
#endif
    dValue = *(double*)&i;
    dValue = dValue*(1.5 - dHalf*dValue*dValue);
    return dValue;
}
//----------------------------------------------------------------------------
}