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

//----------------------------------------------------------------------------
template <int N>
SETInteger<N>::SETInteger(int i)
{
    if( i >= 0 )
    {
        memset(m_asBuffer, 0, TINT_BYTES);
    }
    else
    {
        memset(m_asBuffer, 0xFF, TINT_BYTES);
    }
    SESystem::Memcpy(m_asBuffer, sizeof(int), &i, sizeof(int));
#ifdef SE_BIG_ENDIAN
    short sSave = m_asBuffer[0];
    m_asBuffer[0] = m_asBuffer[1];
    m_asBuffer[1] = sSave;
#endif
}
//----------------------------------------------------------------------------
template <int N>
SETInteger<N>::SETInteger(const SETInteger& rI)
{
    SESystem::Memcpy(m_asBuffer, TINT_BYTES, rI.m_asBuffer, TINT_BYTES);
}
//----------------------------------------------------------------------------
template <int N>
SETInteger<N>::~SETInteger()
{
}
//----------------------------------------------------------------------------
template <int N>
SETInteger<N>& SETInteger<N>::operator=(const SETInteger& rI)
{
    SESystem::Memcpy(m_asBuffer, TINT_BYTES, rI.m_asBuffer, TINT_BYTES);

    return *this;
}
//----------------------------------------------------------------------------
template <int N>
int SETInteger<N>::GetSign() const
{
    return (m_asBuffer[TINT_LAST] & 0x8000) ? -1 : +1;
}
//----------------------------------------------------------------------------
template <int N>
bool SETInteger<N>::operator==(const SETInteger& rI) const
{
    return Compare(*this, rI) == 0;
}
//----------------------------------------------------------------------------
template <int N>
bool SETInteger<N>::operator!=(const SETInteger& rI) const
{
    return Compare(*this, rI) != 0;
}
//----------------------------------------------------------------------------
template <int N>
bool SETInteger<N>::operator<(const SETInteger& rI) const
{
    int iS0 = GetSign(), iS1 = rI.GetSign();
    if( iS0 > 0 )
    {
        if( iS1 > 0 )
        {
            return Compare(*this, rI) < 0;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if( iS1 > 0 )
        {
            return true;
        }
        else
        {
            return Compare(*this, rI) < 0;
        }
    }
}
//----------------------------------------------------------------------------
template <int N>
bool SETInteger<N>::operator<=(const SETInteger& rI) const
{
    int iS0 = GetSign(), iS1 = rI.GetSign();
    if( iS0 > 0 )
    {
        if( iS1 > 0 )
        {
            return Compare(*this, rI) <= 0;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if( iS1 > 0 )
        {
            return true;
        }
        else
        {
            return Compare(*this, rI) <= 0;
        }
    }
}
//----------------------------------------------------------------------------
template <int N>
bool SETInteger<N>::operator>(const SETInteger& rI) const
{
    int iS0 = GetSign(), iS1 = rI.GetSign();
    if( iS0 > 0 )
    {
        if( iS1 > 0 )
        {
            return Compare(*this, rI) > 0;
        }
        else
        {
            return true;
        }
    }
    else
    {
        if( iS1 > 0 )
        {
            return false;
        }
        else
        {
            return Compare(*this, rI) > 0;
        }
    }
}
//----------------------------------------------------------------------------
template <int N>
bool SETInteger<N>::operator>=(const SETInteger& rI) const
{
    int iS0 = GetSign(), iS1 = rI.GetSign();
    if( iS0 > 0 )
    {
        if( iS1 > 0 )
        {
            return Compare(*this, rI) >= 0;
        }
        else
        {
            return true;
        }
    }
    else
    {
        if( iS1 > 0 )
        {
            return false;
        }
        else
        {
            return Compare(*this, rI) >= 0;
        }
    }
}
//----------------------------------------------------------------------------
template <int N>
int SETInteger<N>::Compare(const SETInteger<N>& rI0, const SETInteger<N>& rI1)
{
    for( int i = TINT_LAST; i >= 0; i-- )
    {
        unsigned int uiValue0 = (unsigned int)rI0.m_asBuffer[i];
        unsigned int uiValue1 = (unsigned int)rI1.m_asBuffer[i];
        if( uiValue0 < uiValue1 )
        {
            return -1;
        }
        else if( uiValue0 > uiValue1 )
        {
            return +1;
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
template <int N>
SETInteger<N> SETInteger<N>::operator-() const
{
    SETInteger tempResult = *this;

    // negate the bits
    int i;
    for( i = 0; i < TINT_SIZE; i++ )
    {
        tempResult.m_asBuffer[i] = ~tempResult.m_asBuffer[i];
    }

    // add 1 (place in carry bit and add zero to 'result')
    unsigned int uiCarry = 1;
    for( i = 0; i < TINT_SIZE; i++ )
    {
        unsigned int uiB1 = tempResult.ToUnsignedInt(i);
        unsigned int uiSum = uiB1 + uiCarry;
        tempResult.FromUnsignedInt(i, uiSum);
        uiCarry = (uiSum & 0x00010000) ? 1 : 0;
    }

    // test for overflow
    if( tempResult.GetSign() == GetSign() )
    {
        SE_ASSERT( tempResult == 0 );
    }

    return tempResult;
}
//----------------------------------------------------------------------------
template <int N>
SETInteger<N> SETInteger<N>::operator+(const SETInteger& rI) const
{
    SETInteger tempResult;

    unsigned int uiCarry = 0;
    for( int i = 0; i < TINT_SIZE; i++ )
    {
        unsigned int uiB0 = ToUnsignedInt(i);
        unsigned int uiB1 = rI.ToUnsignedInt(i);
        unsigned int uiSum = uiB0 + uiB1 + uiCarry;
        tempResult.FromUnsignedInt(i, uiSum);
        uiCarry = (uiSum & 0x00010000) ? 1 : 0;
    }

    // test for overflow
    if( GetSign() == rI.GetSign() )
    {
        SE_ASSERT(tempResult.GetSign() == GetSign());
    }

    return tempResult;
}
//----------------------------------------------------------------------------
template <int N>
SETInteger<N> SETInteger<N>::operator-(const SETInteger& rI) const
{
    return *this + (-rI);
}
//----------------------------------------------------------------------------
template <int N>
SETInteger<N> SETInteger<N>::operator*(const SETInteger& rI) const
{
    int iS0 = GetSign(), iS1 = rI.GetSign(), iSProduct = iS0 * iS1;
    SETInteger tempOp0 = (iS0 > 0 ? *this : -*this);
    SETInteger tempOp1 = (iS1 > 0 ? rI : -rI);

    // product of single-digit number with multiple-digit number
    unsigned short ausProduct[2 * TINT_SIZE];
    unsigned short* pusPCurrent = ausProduct;

    // product of the two multiple-digit operands
    unsigned short ausResult[2 * TINT_SIZE];
    unsigned short* pusRCurrent = ausResult;
    memset(ausResult, 0, 2 * TINT_BYTES);

    for( int i0 = 0, iSize = 2 * TINT_SIZE; i0 < TINT_SIZE; i0++, iSize-- )
    {
        unsigned int uiB0 = tempOp0.ToUnsignedInt(i0);
        if( uiB0 > 0 )
        {
            unsigned short* pusPBuffer = pusPCurrent;
            unsigned int uiCarry = 0;
            int i1;
            for( i1 = 0; i1 < TINT_SIZE; i1++ )
            {
                unsigned int uiB1 = tempOp1.ToUnsignedInt(i1);
                unsigned int uiProd = uiB0*uiB1 + uiCarry;
                *pusPBuffer++ = (unsigned short)(uiProd & 0x0000FFFF);
                uiCarry = (uiProd & 0xFFFF0000) >> 16;
            }
            *pusPBuffer = (unsigned short)uiCarry;

            unsigned short* pusRBuffer = pusRCurrent;
            pusPBuffer = pusPCurrent;
            uiCarry = 0;
            unsigned int uiSum, uiTerm0, uiTerm1;
            for( i1 = 0; i1 <= TINT_SIZE; i1++ )
            {
                uiTerm0 = (unsigned int)(*pusPBuffer++);
                uiTerm1 = (unsigned int)(*pusRBuffer);
                uiSum = uiTerm0 + uiTerm1 + uiCarry;
                *pusRBuffer++ = (unsigned short)(uiSum & 0x0000FFFF);
                uiCarry = (uiSum & 0x00010000) ? 1 : 0;
            }

            for( /**/; uiCarry > 0 && i1 < iSize; i1++ )
            {
                uiTerm0 = (unsigned int)(*pusRBuffer);
                uiSum = uiTerm0 + uiCarry;
                *pusRBuffer++ = (unsigned short)(uiSum & 0x0000FFFF);
                uiCarry = (uiSum & 0x00010000) ? 1 : 0;
            }
        }

        pusPCurrent++;
        pusRCurrent++;
    }

    // Test for overflow.  You can test earlier inside the previous loop, but
    // testing here allows you to get an idea of how much overflow there is.
    // This information might be useful for an application to decide how large
    // to choose the integer size.
    for( int i = 2*TINT_SIZE - 1; i >= TINT_SIZE; i-- )
    {
        SE_ASSERT( ausResult[i] == 0 );
    }
    SE_ASSERT( (ausResult[TINT_LAST] & 0x8000) == 0 );

    SETInteger tempResult;
    SESystem::Memcpy(tempResult.m_asBuffer, TINT_BYTES, ausResult, 
        TINT_BYTES);
    if( iSProduct < 0 )
    {
        tempResult = -tempResult;
    }

    return tempResult;
}
//----------------------------------------------------------------------------
template <int N>
SETInteger<N> operator*(int i, const SETInteger<N>& rI)
{
    return rI * i;
}
//----------------------------------------------------------------------------
template <int N>
SETInteger<N> SETInteger<N>::operator/(const SETInteger& rI) const
{
    // TO DO.  On division by zero, return INVALID or signed INFINITY?
    SETInteger tempQ, tempR;
    return (GetDivMod(*this, rI, tempQ, tempR) ? tempQ : 0);
}
//----------------------------------------------------------------------------
template <int N>
SETInteger<N> SETInteger<N>::operator%(const SETInteger& rI) const
{
    // TO DO.  On division by zero, return INVALID or signed INFINITY?
    SETInteger tempQ, tempR;
    return (GetDivMod(*this, rI, tempQ, tempR) ? tempR : 0);
}
//----------------------------------------------------------------------------
template <int N>
SETInteger<N>& SETInteger<N>::operator+=(const SETInteger& rI)
{
    *this = *this + rI;
    return *this;
}
//----------------------------------------------------------------------------
template <int N>
SETInteger<N>& SETInteger<N>::operator-=(const SETInteger& rI)
{
    *this = *this - rI;
    return *this;
}
//----------------------------------------------------------------------------
template <int N>
SETInteger<N>& SETInteger<N>::operator*=(const SETInteger& rI)
{
    *this = *this * rI;
    return *this;
}
//----------------------------------------------------------------------------
template <int N>
SETInteger<N>& SETInteger<N>::operator/=(const SETInteger& rI)
{
    *this = *this / rI;
    return *this;
}
//----------------------------------------------------------------------------
template <int N>
SETInteger<N> SETInteger<N>::operator<<(int iShift) const
{
    if( iShift < 0 )
    {
        return 0;
    }
    if( iShift == 0 )
    {
        return *this;
    }

    // number of 16-bit blocks to shift
    SETInteger tempResult;
    int iBlocks = iShift / 16;
    if( iBlocks > TINT_LAST )
    {
        return 0;
    }

    int i;
    if( iBlocks > 0 )
    {
        int j = TINT_LAST - iBlocks;
        for( i = TINT_LAST; j >= 0; i--, j-- )
        {
            tempResult.m_asBuffer[i] = m_asBuffer[j];
        }

        for( /**/; i >= 0; i-- )
        {
            tempResult.m_asBuffer[i] = 0;
        }
    }

    // number of left-over bits to shift
    int iBits = iShift % 16;
    if( iBits > 0 )
    {
        unsigned int uiLo, uiHi, uiValue;
        int iM1;
        for( i = TINT_LAST, iM1 = i-1; iM1 >= 0; i--, iM1-- )
        {
            uiLo = ToUnsignedInt(iM1);
            uiHi = ToUnsignedInt(i);
            uiValue = (uiLo | (uiHi << 16));
            uiValue <<= iBits;
            tempResult.FromUnsignedInt(i, ((0xFFFF0000 & uiValue) >> 16));
        }

        uiValue = ToUnsignedInt(0);
        uiValue <<= iBits;
        tempResult.FromUnsignedInt(0, (0x0000FFFF & uiValue));
    }

    return tempResult;
}
//----------------------------------------------------------------------------
template <int N>
SETInteger<N> SETInteger<N>::operator>>(int iShift) const
{
    if( iShift < 0 )
    {
        return 0;
    }
    if( iShift == 0 )
    {
        return *this;
    }

    // number of 16-bit blocks to shift
    SETInteger tempResult;
    int iBlocks = iShift/16;
    if( iBlocks > TINT_LAST )
    {
        return 0;
    }

    int i;
    if( iBlocks > 0 )
    {
        int j = iBlocks;
        for( i = 0; j <= TINT_LAST; i++, j++ )
        {
            tempResult.m_asBuffer[i] = m_asBuffer[j];
        }

        if( GetSign() > 0 )
        {
            for( /**/; i <= TINT_LAST; i++ )
            {
                tempResult.m_asBuffer[i] = 0;
            }
        }
        else
        {
            for( /**/; i <= TINT_LAST; i++ )
            {
                tempResult.m_asBuffer[i] = (short)(0x0000FFFF);
            }
        }
    }

    // number of left-over bits to shift
    int iBits = iShift % 16;
    if( iBits > 0 )
    {
        unsigned int uiValue;
        int iP1;
        for( i = 0, iP1 = 1; iP1 <= TINT_LAST; i++, iP1++ )
        {
            uiValue = ToUnsignedInt(i, iP1);
            uiValue >>= iBits;
            tempResult.FromUnsignedInt(i, uiValue);
        }

        uiValue = ToUnsignedInt(TINT_LAST);
        if( GetSign() < 0 )
        {
            uiValue |= 0xFFFF0000;  // sign extension
        }
        uiValue >>= iBits;
        tempResult.FromUnsignedInt(TINT_LAST, uiValue);
    }

    return tempResult;
}
//----------------------------------------------------------------------------
template <int N>
SETInteger<N>& SETInteger<N>::operator<<=(int iShift)
{
    if( iShift <= 0 )
    {
        return *this;
    }

    // number of 16-bit blocks to shift
    SETInteger tempResult;
    int iBlocks = iShift / 16;
    if( iBlocks > TINT_LAST )
    {
        return *this;
    }

    int i;
    if( iBlocks > 0 )
    {
        int j = TINT_LAST - iBlocks;
        for( i = TINT_LAST; j >= 0; i--, j-- )
        {
            m_asBuffer[i] = m_asBuffer[j];
        }

        for( /**/; i >= 0; i-- )
        {
            m_asBuffer[i] = 0;
        }
    }

    // number of left-over bits to shift
    int iBits = iShift % 16;
    if( iBits > 0 )
    {
        unsigned int uiValue;
        int iM1;
        for( i = TINT_LAST, iM1 = i-1; iM1 >= 0; i--, iM1-- )
        {
            uiValue = ToUnsignedInt(iM1, i);
            uiValue <<= iBits;
            FromUnsignedInt(i, ((0xFFFF0000 & uiValue) >> 16));
        }

        uiValue = ToUnsignedInt(0);
        uiValue <<= iBits;
        FromUnsignedInt(0, (0x0000FFFF & uiValue));
    }

    return *this;
}
//----------------------------------------------------------------------------
template <int N>
SETInteger<N>& SETInteger<N>::operator>>=(int iShift)
{
    if( iShift <= 0 )
    {
        return *this;
    }

    // number of 16-bit blocks to shift
    int iBlocks = iShift / 16;
    if( iBlocks > TINT_LAST )
    {
        return *this;
    }

    int i;
    if( iBlocks > 0 )
    {
        int j = iBlocks;
        for( i = 0, j = iBlocks; j <= TINT_LAST; i++, j++ )
        {
            m_asBuffer[i] = m_asBuffer[j];
        }

        if( GetSign() > 0 )
        {
            for( /**/; i <= TINT_LAST; i++ )
            {
                m_asBuffer[i] = 0;
            }
        }
        else
        {
            for( /**/; i <= TINT_LAST; i++ )
            {
                m_asBuffer[i] = -1;
            }
        }
    }

    // number of left-over bits to shift
    int iBits = iShift % 16;
    if( iBits > 0 )
    {
        unsigned int uiValue;
        int iP1;
        for( i = 0, iP1 = 1; iP1 <= TINT_LAST; i++, iP1++ )
        {
            uiValue = ToUnsignedInt(i, iP1);
            uiValue >>= iBits;
            FromUnsignedInt(i, uiValue);
        }

        uiValue = ToUnsignedInt(TINT_LAST);
        if( GetSign() < 0 )
        {
            uiValue |= 0xFFFF0000;  // sign extension
        }
        uiValue >>= iBits;
        FromUnsignedInt(TINT_LAST, uiValue);
    }

    return *this;
}
//----------------------------------------------------------------------------
template <int N>
bool SETInteger<N>::GetDivMod(const SETInteger& rNumer, const SETInteger& 
    rDenom, SETInteger& rQuo, SETInteger& rRem)
{
    if( rDenom == 0 )
    {
        SE_ASSERT( false );

        rQuo = 0;
        rRem = 0;

        return false;
    }

    if( rNumer == 0 )
    {
        rQuo = 0;
        rRem = 0;

        return true;
    }

    // work with the absolute values of the numerator and denominator
    int iS0 = rNumer.GetSign(), iS1 = rDenom.GetSign();
    SETInteger tempAbsNumer = iS0*rNumer;
    SETInteger tempAbsDenom = iS1*rDenom;

    int iCompare = Compare(tempAbsNumer, tempAbsDenom);
    if( iCompare < 0 )
    {
        // numerator < denominator:  numerator = 0*denominator + numerator
        rQuo = 0;
        rRem = rNumer;

        return true;
    }

    if( iCompare == 0)
    {
        // numerator == denominator:  numerator = 1*denominator + 0
        rQuo = 1;
        rRem = 0;
        return true;
    }

    // numerator > denominator, do the division to find quotient and remainder
    if( tempAbsDenom > 0x0000FFFF )
    {
        DivMultiple(tempAbsNumer, tempAbsDenom, rQuo, rRem);
    }
    else
    {
        DivSingle(tempAbsNumer, tempAbsDenom.m_asBuffer[0], rQuo, rRem);
    }

    // apply the original signs of numerator and denominator
    rQuo *= iS0 * iS1;
    rRem *= iS0;

#ifdef _DEBUG
    SETInteger tempTest = rNumer - rDenom*rQuo - rRem;
    SE_ASSERT( tempTest == 0 );
#endif
    return true;
}
//----------------------------------------------------------------------------
template <int N>
void SETInteger<N>::DivSingle(const SETInteger& rNumer, short sDenom, 
    SETInteger& rQuo, SETInteger& rRem)
{
    // denominator is a single "digit"
    unsigned int uiDenom = 0x0000FFFF & (unsigned int)sDenom;

    // numerator
    int iNStart = rNumer.GetLeadingBlock();
    const short* psNumer = &rNumer.m_asBuffer[iNStart];
    unsigned int uiDigit1 = 0;

    // quotient
    short* psQuo = &rQuo.m_asBuffer[iNStart];
    rQuo = 0;
    int iLastNonZero = -1;
    for( int i = iNStart; i >= 0; i--, psNumer--, psQuo-- )
    {
        unsigned int uiDigitB = uiDigit1;
        uiDigit1 = 0x0000FFFF & (unsigned int)(*psNumer);
        unsigned int uiNumer = (uiDigitB << 16) | uiDigit1;
        unsigned int uiQuo = uiNumer/uiDenom;
        uiDigit1 = uiNumer - uiQuo*uiDenom;
        *psQuo = (short)(uiQuo & 0x0000FFFF);
        if( iLastNonZero == -1 && uiQuo > 0 )
        {
            iLastNonZero = i;
        }
    }
    SE_ASSERT( iLastNonZero >= 0 );

    // remainder
    size_t uiSize;
    rRem = 0;
    if( uiDigit1 & 0xFFFF0000 )
    {
        uiSize = 2*sizeof(short);
        SESystem::Memcpy(rRem.m_asBuffer, uiSize, &uiDigit1, uiSize);
#ifdef SE_BIG_ENDIAN
        short sSave = rRem.m_asBuffer[0];
        rRem.m_asBuffer[0] = rRem.m_asBuffer[1];
        rRem.m_asBuffer[1] = sSave;
#endif
    }
    else
    {
        unsigned short usDigit1 = (unsigned short)uiDigit1;
        uiSize = sizeof(short);
        SESystem::Memcpy(rRem.m_asBuffer, uiSize, &usDigit1, uiSize);
    }
}
//----------------------------------------------------------------------------
template <int N>
void SETInteger<N>::DivMultiple(const SETInteger& rNumer, 
    const SETInteger& rDenom, SETInteger& rQuo, SETInteger& rRem)
{
    rQuo = 0;
    rRem = 0;

    // Normalization to allow good estimate of quotient.  TO DO:  It is
    // possible that the numerator is large enough that normalization causes
    // overflow when computing the product iAdjust*rNumer; an assertion will
    // fire in this case.  Ideally the overflow would be allowed and the
    // digit in the overflow position becomes the first digit of the numerator
    // in the division algorithm. This will require a mixture of SETInteger<N>
    // and SETInteger<N+1>, though.
    int iDInit = rDenom.GetLeadingBlock();
    int iLeadingDigit = rDenom.ToInt(iDInit);
    int iAdjust = 0x10000/(iLeadingDigit + 1);
    SETInteger tempANum = iAdjust * rNumer;
    SETInteger tempADen = iAdjust * rDenom;
    SE_ASSERT( tempADen.GetLeadingBlock() == iDInit );

    // get first two "digits" of denominator
    unsigned int uiD1 = tempADen.ToUnsignedInt(iDInit);
    unsigned int uiD2 = tempADen.ToUnsignedInt(iDInit - 1);

    // determine the maximum necessary division steps
    int iNInit = tempANum.GetLeadingBlock();
    SE_ASSERT( iNInit >= iDInit );
    int iQInit;
    unsigned int uiRHat;
    if( iNInit != iDInit )
    {
        iQInit = iNInit-iDInit-1;
        uiRHat = 1;
    }
    else
    {
        iQInit = 0;
        uiRHat = 0;
    }

    for( /**/; iQInit >= 0; iQInit-- )
    {
        // get first three indices of remainder
        unsigned int uiN0, uiN1, uiN2;
        if( uiRHat > 0 )
        {
            uiN0 = tempANum.ToUnsignedInt(iNInit--);
            uiN1 = tempANum.ToUnsignedInt(iNInit--);
            uiN2 = tempANum.ToUnsignedInt(iNInit);
        }
        else
        {
            uiN0 = 0;
            uiN1 = tempANum.ToUnsignedInt(iNInit--);
            uiN2 = tempANum.ToUnsignedInt(iNInit);
        }

        // estimate the quotient
        unsigned int uiTmp = (uiN0 << 16) | uiN1;
        unsigned int uiQHat = (uiN0 != uiD1 ? uiTmp/uiD1 : 0x0000FFFF);
        unsigned int uiProd = uiQHat * uiD1;
        SE_ASSERT( uiTmp >= uiProd );
        uiRHat = uiTmp - uiProd;
        if( uiD2*uiQHat > 0x10000*uiRHat + uiN2 )
        {
            uiQHat--;
            uiRHat += uiD1;
            if( uiD2*uiQHat > 0x10000*uiRHat + uiN2 )
            {
                // If this block is entered, we have exactly the quotient for
                // the division.  The adjustment block of code later cannot
                // happen.
                uiQHat--;
                uiRHat += uiD1;
            }
        }

        // compute the quotient for this step of the division
        SETInteger tempLocalQuo;
        tempLocalQuo.FromUnsignedInt(iQInit, uiQHat);

        // compute the remainder
        SETInteger tempProduct = tempLocalQuo*tempADen;
        tempANum -= tempProduct;
        if( tempANum < 0 )
        {
            uiQHat--;
            tempANum += tempADen;
            SE_ASSERT( tempANum >= 0 );
        }

        // set quotient digit
        rQuo.FromUnsignedInt(iQInit, uiQHat);

        if( tempANum >= tempADen )
        {
            // prepare to do another division step
            iNInit = tempANum.GetLeadingBlock();
        }
        else
        {
            // remainder is smaller than divisor, finished dividing
            break;
        }
    }

    // unnormalize the remainder
    if( tempANum > 0)
    {
        short sDivisor = (short)(iAdjust & 0x0000FFFF);
        SETInteger kShouldBeZero;
        DivSingle(tempANum, sDivisor, rRem, kShouldBeZero);
    }
    else
    {
        rRem = 0;
    }
}
//----------------------------------------------------------------------------
template <int N>
int SETInteger<N>::GetLeadingBlock() const
{
    for( int i = TINT_LAST; i >= 0; i-- )
    {
        if( m_asBuffer[i] != 0 )
        {
            return i;
        }
    }

    return -1;
}
//----------------------------------------------------------------------------
template <int N>
int SETInteger<N>::GetTrailingBlock() const
{
    for( int i = 0; i <= TINT_LAST; i++ )
    {
        if( m_asBuffer[i] != 0 )
        {
            return i;
        }
    }

    return -1;
}
//----------------------------------------------------------------------------
template <int N>
int SETInteger<N>::GetLeadingBit(int i) const
{
    SE_ASSERT( 0 <= i && i <= TINT_LAST );

    if( i < 0 || i > TINT_LAST )
    {
        return -1;
    }

    // This is a binary search for the high-order bit of m_asBuffer[i].  The
    // return value is the index into the bits (0 <= index < 16).
    int iValue = (int)m_asBuffer[i];
    if( (iValue & 0xFF00) != 0 )
    {
        if( (iValue & 0xF000) != 0 )
        {
            if( (iValue & 0xC000) != 0 )
            {
                if( (iValue & 0x8000) != 0 )
                {
                    return 15;
                }
                else // (iValue & 0x4000) != 0
                {
                    return 14;
                }
            }
            else  // (iValue & 0x3000) != 0
            {
                if( (iValue & 0x2000) != 0)
                {
                    return 13;
                }
                else  // (iValue & 0x1000) != 0
                {
                    return 12;
                }
            }
        }
        else  // (iValue & 0x0F00) != 0
        {
            if( (iValue & 0x0C00) != 0 )
            {
                if( (iValue & 0x0800) != 0 )
                {
                    return 11;
                }
                else  // (iValue & 0x0400) != 0
                {
                    return 10;
                }
            }
            else  // (iValue & 0x0300) != 0
            {
                if( (iValue & 0x0200) != 0 )
                {
                    return 9;
                }
                else  // (iValue & 0x0100) != 0
                {
                    return 8;
                }
            }
        }
    }
    else  // (iValue & 0x00FF)
    {
        if( (iValue & 0x00F0) != 0 )
        {
            if( (iValue & 0x00C0) != 0 )
            {
                if( (iValue & 0x0080) != 0 )
                {
                    return 7;
                }
                else  // (iValue & 0x0040) != 0
                {
                    return 6;
                }
            }
            else  // (iValue & 0x0030) != 0
            {
                if( (iValue & 0x0020) != 0 )
                {
                    return 5;
                }
                else  // (iValue & 0x0010) != 0
                {
                    return 4;
                }
            }
        }
        else  // (iValue & 0x000F) != 0
        {
            if( (iValue & 0x000C) != 0 )
            {
                if( (iValue & 0x0008) != 0 )
                {
                    return 3;
                }
                else  // (iValue & 0x0004) != 0
                {
                    return 2;
                }
            }
            else  // (iValue & 0x0003) != 0
            {
                if( (iValue & 0x0002) != 0 )
                {
                    return 1;
                }
                else  // (iValue & 0x0001) != 0
                {
                    return 0;
                }
            }
        }
    }

    return -1;
}
//----------------------------------------------------------------------------
template <int N>
int SETInteger<N>::GetTrailingBit(int i) const
{
    SE_ASSERT( 0 <= i && i <= TINT_LAST );

    if( i < 0 || i > TINT_LAST )
    {
        return -1;
    }

    // This is a binary search for the low-order bit of m_asBuffer[i].  The
    // return value is the index into the bits (0 <= index < 16).
    int iValue = (int)m_asBuffer[i];
    if( (iValue & 0x00FF) != 0 )
    {
        if( (iValue & 0x000F) != 0 )
        {
            if( (iValue & 0x0003) != 0 )
            {
                if( (iValue & 0x0001) != 0 )
                {
                    return 0;
                }
                else // (iValue & 0x0002) != 0
                {
                    return 1;
                }
            }
            else  // (iValue & 0x000C) != 0
            {
                if( (iValue & 0x0004) != 0 )
                {
                    return 2;
                }
                else  // (iValue & 0x0080) != 0
                {
                    return 3;
                }
            }
        }
        else  // (iValue & 0x00F0) != 0
        {
            if( (iValue & 0x0030) != 0 )
            {
                if( (iValue & 0x0010) != 0 )
                {
                    return 4;
                }
                else  // (iValue & 0x0020) != 0
                {
                    return 5;
                }
            }
            else  // (iValue & 0x00C0) != 0
            {
                if( (iValue & 0x0040) != 0 )
                {
                    return 6;
                }
                else  // (iValue & 0x0080) != 0
                {
                    return 7;
                }
            }
        }
    }
    else  // (iValue & 0xFF00)
    {
        if( (iValue & 0x0F00) != 0 )
        {
            if( (iValue & 0x0300) != 0 )
            {
                if( (iValue & 0x0100) != 0 )
                {
                    return 8;
                }
                else  // (iValue & 0x0200) != 0
                {
                    return 9;
                }
            }
            else  // (iValue & 0x0C00) != 0
            {
                if( (iValue & 0x0400) != 0 )
                {
                    return 10;
                }
                else  // (iValue & 0x0800) != 0
                {
                    return 11;
                }
            }
        }
        else  // (iValue & 0xF000) != 0
        {
            if( (iValue & 0x3000) != 0 )
            {
                if( (iValue & 0x1000) != 0 )
                {
                    return 12;
                }
                else  // (iValue & 0x2000) != 0
                {
                    return 13;
                }
            }
            else  // (iValue & 0xC000) != 0
            {
                if( (iValue & 0x4000) != 0 )
                {
                    return 14;
                }
                else  // (iValue & 0x8000) != 0
                {
                    return 15;
                }
            }
        }
    }
}
//----------------------------------------------------------------------------
template <int N>
int SETInteger<N>::GetLeadingBit() const
{
    int iBlock = GetLeadingBlock();
    if( iBlock >= 0 )
    {
        int iBit = GetLeadingBit(iBlock);
        if( iBit >= 0 )
        {
            return iBit + 16*iBlock;
        }
    }

    return -1;
}
//----------------------------------------------------------------------------
template <int N>
int SETInteger<N>::GetTrailingBit() const
{
    int iBlock = GetTrailingBlock();
    if( iBlock >= 0 )
    {
        int iBit = GetTrailingBit(iBlock);
        if( iBit >= 0 )
        {
            return iBit + 16*iBlock;
        }
    }

    return -1;
}
//----------------------------------------------------------------------------
template <int N>
void SETInteger<N>::SetBit(int i, bool bOn)
{
    // SE_ASSERT( 0 <= i && i <= TINT_LAST );
    int iBlock = i / 16;
    int iBit = i % 16;
    if( bOn )
    {
        m_asBuffer[iBlock] |= (1 << iBit);
    }
    else
    {
        m_asBuffer[iBlock] &= ~(1 << iBit);
    }
}
//----------------------------------------------------------------------------
template <int N>
bool SETInteger<N>::GetBit(int i) const
{
    // SE_ASSERT( 0 <= i && i <= TINT_LAST );
    int iBlock = i / 16;
    int iBit = i % 16;

    return (m_asBuffer[iBlock] & (1 << iBit)) != 0;
}
//----------------------------------------------------------------------------
template <int N>
unsigned int SETInteger<N>::ToUnsignedInt(int i) const
{
    // SE_ASSERT( 0 <= i && i <= TINT_LAST );

    return 0x0000FFFF & (unsigned int)m_asBuffer[i];
}
//----------------------------------------------------------------------------
template <int N>
void SETInteger<N>::FromUnsignedInt(int i, unsigned int uiValue)
{
    // SE_ASSERT( 0 <= i && i <= TINT_LAST );

    m_asBuffer[i] = (short)(uiValue & 0x0000FFFF);
}
//----------------------------------------------------------------------------
template <int N>
unsigned int SETInteger<N>::ToUnsignedInt(int iLo, int iHi) const
{
    unsigned int uiLo = ToUnsignedInt(iLo);
    unsigned int uiHi = ToUnsignedInt(iHi);

    return (uiLo | (uiHi << 16));
}
//----------------------------------------------------------------------------
template <int N>
int SETInteger<N>::ToInt(int i) const
{
    // SE_ASSERT( 0 <= i && i <= TINT_LAST );

    return (int)(0x0000FFFF & (unsigned int)m_asBuffer[i]);
}
//----------------------------------------------------------------------------

