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

#ifndef Swing_TInteger_H
#define Swing_TInteger_H

#include "SEGeometricToolsLIB.h"
#include "SESystem.h"

namespace Swing
{

template <int N> class SETRational;

//----------------------------------------------------------------------------
// Description:N is the number of 32-bit words you want per SETInteger.
// Date:20081201
//----------------------------------------------------------------------------
template <int N>
class SETInteger
{
public:
    // construction and destruction
    SETInteger(int i = 0);
    SETInteger(const SETInteger& rI);
    ~SETInteger(void);

    // assignment
    SETInteger& operator = (const SETInteger& rI);

    // comparison
    bool operator == (const SETInteger& rI) const;
    bool operator != (const SETInteger& rI) const;
    bool operator <  (const SETInteger& rI) const;
    bool operator <= (const SETInteger& rI) const;
    bool operator >  (const SETInteger& rI) const;
    bool operator >= (const SETInteger& rI) const;

    // arithmetic operations
    SETInteger operator - (void) const;
    SETInteger operator + (const SETInteger& rI) const;
    SETInteger operator - (const SETInteger& rI) const;
    SETInteger operator * (const SETInteger& rI) const;
    SETInteger operator / (const SETInteger& rI) const;
    SETInteger operator % (const SETInteger& rI) const;

    // arithmetic updates
    SETInteger& operator += (const SETInteger& rI);
    SETInteger& operator -= (const SETInteger& rI);
    SETInteger& operator *= (const SETInteger& rI);
    SETInteger& operator /= (const SETInteger& rI);

    // shift operations
    SETInteger operator << (int iShift) const;
    SETInteger operator >> (int iShift) const;

    // shift updates
    SETInteger& operator <<= (int iShift);
    SETInteger& operator >>= (int iShift);

private:
    // Support for comparisons. The return value of Compare is -1 if I0 < I1, 
    // is 0 if I0 == I1,  or is +1 if I0 > I1.
    static int Compare(const SETInteger& rI0, const SETInteger& rI1);
    int GetSign(void) const;

    // support for division and modulo
    static bool GetDivMod(const SETInteger& rNumer, const SETInteger& rDenom, 
        SETInteger& rQuotient, SETInteger& rRemainder);

    static void DivSingle(const SETInteger& rNumer, short usDenom, 
        SETInteger& rQuo, SETInteger& rRem);

    static void DivMultiple(const SETInteger& rNumer, const SETInteger& rDenom, 
        SETInteger& rQuo, SETInteger& rRem);

    // miscellaneous utilities
    int GetLeadingBlock(void) const;
    int GetTrailingBlock(void) const;
    int GetLeadingBit(int i) const;  // of m_asBuffer[i]
    int GetTrailingBit(int i) const;  // of m_asBuffer[i]
    int GetLeadingBit(void) const;  // of entire number
    int GetTrailingBit(void) const;  // of entire number
    void SetBit(int i,  bool bOn);
    bool GetBit(int i) const;
    unsigned int ToUnsignedInt(int i) const;
    void FromUnsignedInt(int i,  unsigned int uiValue);
    unsigned int ToUnsignedInt(int iLo,  int iHi) const;
    int ToInt(int i) const;

    enum
    {
        TINT_SIZE = 2 * N, 
        TINT_BYTES = TINT_SIZE * sizeof(short), 
        TINT_LAST = TINT_SIZE - 1
    };

    short m_asBuffer[TINT_SIZE];

    // SETRational needs access to private members of SETInteger.
    friend class SETRational<N>;
};

template <int N>
SETInteger<N> operator * (int i,  const SETInteger<N>& rI);

#include "SETInteger.inl"

}

#endif
