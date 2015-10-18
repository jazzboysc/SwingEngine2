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

#ifndef Swing_TRational_H
#define Swing_TRational_H

#include "SEGeometricToolsLIB.h"
#include "SETInteger.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:N is the number of 32-bit words per SETInteger numerator/
//     denominator.
// Date:20081202
//----------------------------------------------------------------------------
template <int N>
class SETRational
{
public:
    // construction
    SETRational(void);  // default rational is 0/1
    SETRational(const SETRational& rR);
    SETRational(const SETInteger<N>& rNumer);
    SETRational(const SETInteger<N>& rNumer,  const SETInteger<N>& rDenom);

    // construction converters
    SETRational(int iNumer);
    SETRational(int iNumer,  int iDenom);
    SETRational(float fValue);
    SETRational(double dValue);

    // member access
    SETInteger<N>& Numer(void);
    SETInteger<N>& Denom(void);
    const SETInteger<N>& Numer(void) const;
    const SETInteger<N>& Denom(void) const;

    // assignment
    SETRational& operator = (const SETRational& rR);

    // comparison
    bool operator == (const SETRational& rR) const;
    bool operator != (const SETRational& rR) const;
    bool operator <= (const SETRational& rR) const;
    bool operator <  (const SETRational& rR) const;
    bool operator >= (const SETRational& rR) const;
    bool operator >  (const SETRational& rR) const;

    // arithmetic operations
    SETRational operator + (const SETRational& rR) const;
    SETRational operator - (const SETRational& rR) const;
    SETRational operator * (const SETRational& rR) const;
    SETRational operator / (const SETRational& rR) const;
    SETRational operator - (void) const;

    // arithmetic updates
    SETRational& operator += (const SETRational& rR);
    SETRational& operator -= (const SETRational& rR);
    SETRational& operator *= (const SETRational& rR);
    SETRational& operator /= (const SETRational& rR);

    // conversions to float and double
    void ConvertTo(float& rfValue) const;
    void ConvertTo(double& rdValue) const;

    // compute the absolute value of the rational number
    SETRational Abs(void) const;

private:
    // miscellaneous utilities
    void EliminatePowersOfTwo(void);

    static void GetPositiveFloat(const SETInteger<N>& rDenom, 
        SETInteger<N>& rQuo,  SETInteger<N>& rRem,  int iBlock, 
        unsigned int& ruiExponent,  unsigned int& ruiMantissa);

    static void GetPositiveDouble(const SETInteger<N>& rDenom, 
        SETInteger<N>& rQuo,  SETInteger<N>& rRem,  int iBlock, 
        unsigned int& ruiExponent,  unsigned int& ruiMantissaHi, 
        unsigned int& ruiMantissaLo);

    // 分子与分母.
    SETInteger<N> m_Numer,  m_Denom;
};

template <int N>
SETRational<N> operator + (const SETInteger<N>& rI, const SETRational<N>& rR);

template <int N>
SETRational<N> operator - (const SETInteger<N>& rI, const SETRational<N>& rR);

template <int N>
SETRational<N> operator * (const SETInteger<N>& rI, const SETRational<N>& rR);

template <int N>
SETRational<N> operator / (const SETInteger<N>& rI, const SETRational<N>& rR);

#include "SETRational.inl"

}

#endif
