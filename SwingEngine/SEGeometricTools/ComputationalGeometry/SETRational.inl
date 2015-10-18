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
SETRational<N>::SETRational()
    :
    m_Numer(0), 
    m_Denom(1)
{
    // default construction produces the zero rational number
}
//----------------------------------------------------------------------------
template <int N>
SETRational<N>::SETRational(const SETRational& rR)
    :
    m_Numer(rR.m_Numer), 
    m_Denom(rR.m_Denom)
{
}
//----------------------------------------------------------------------------
template <int N>
SETRational<N>::SETRational(const SETInteger<N>& rNumer)
    :
    m_Numer(rNumer), 
    m_Denom(1)
{
}
//----------------------------------------------------------------------------
template <int N>
SETRational<N>::SETRational(const SETInteger<N>& rNumer, 
    const SETInteger<N>& rDenom)
    :
    m_Numer(rNumer), 
    m_Denom(rDenom)
{
}
//----------------------------------------------------------------------------
template <int N>
SETRational<N>::SETRational(int iNumer)
    :
    m_Numer(iNumer), 
    m_Denom(1)
{
}
//----------------------------------------------------------------------------
template <int N>
SETRational<N>::SETRational(int iNumer,  int iDenom)
    :
    m_Numer(iNumer), 
    m_Denom(iDenom)
{
}
//----------------------------------------------------------------------------
template <int N>
SETInteger<N>& SETRational<N>::Numer()
{
    return m_Numer;
}
//----------------------------------------------------------------------------
template <int N>
SETInteger<N>& SETRational<N>::Denom()
{
    return m_Denom;
}
//----------------------------------------------------------------------------
template <int N>
const SETInteger<N>& SETRational<N>::Numer() const
{
    return m_Numer;
}
//----------------------------------------------------------------------------
template <int N>
const SETInteger<N>& SETRational<N>::Denom() const
{
    return m_Denom;
}
//----------------------------------------------------------------------------
template <int N>
SETRational<N>& SETRational<N>::operator=(const SETRational& rR)
{
    m_Numer = rR.m_Numer;
    m_Denom = rR.m_Denom;

    return *this;
}
//----------------------------------------------------------------------------
template <int N>
bool SETRational<N>::operator==(const SETRational& rR) const
{
    return (m_Numer*rR.m_Denom == m_Denom*rR.m_Numer);
}
//----------------------------------------------------------------------------
template <int N>
bool SETRational<N>::operator!=(const SETRational& rR) const
{
    return (m_Numer*rR.m_Denom != m_Denom*rR.m_Numer);
}
//----------------------------------------------------------------------------
template <int N>
bool SETRational<N>::operator<=(const SETRational& rR) const
{
    SETInteger<N> tempProd0 = m_Numer * rR.m_Denom;
    SETInteger<N> tempProd1 = m_Denom * rR.m_Numer;
    if( m_Denom > 0 )
    {
        return (rR.m_Denom > 0 ? tempProd0 <= tempProd1 : tempProd0 >= 
            tempProd1);
    }
    else
    {
        return (rR.m_Denom > 0 ? tempProd0 >= tempProd1 : tempProd0 <= 
            tempProd1);
    }
}
//----------------------------------------------------------------------------
template <int N>
bool SETRational<N>::operator<(const SETRational& rR) const
{
    SETInteger<N> tempProd0 = m_Numer * rR.m_Denom;
    SETInteger<N> tempProd1 = m_Denom * rR.m_Numer;
    if( m_Denom > 0 )
    {
        return (rR.m_Denom > 0 ? tempProd0 < tempProd1 : tempProd0 > 
            tempProd1);
    }
    else
    {
        return (rR.m_Denom > 0 ? tempProd0 > tempProd1 : tempProd0 < 
            tempProd1);
    }
}
//----------------------------------------------------------------------------
template <int N>
bool SETRational<N>::operator>=(const SETRational& rR) const
{
    SETInteger<N> tempProd0 = m_Numer * rR.m_Denom;
    SETInteger<N> tempProd1 = m_Denom * rR.m_Numer;
    if( m_Denom > 0 )
    {
        return (rR.m_Denom > 0 ? tempProd0 >= tempProd1 : tempProd0 <= 
            tempProd1);
    }
    else
    {
        return (rR.m_Denom > 0 ? tempProd0 <= tempProd1 : tempProd0 >= 
            tempProd1);
    }
}
//----------------------------------------------------------------------------
template <int N>
bool SETRational<N>::operator>(const SETRational& rR) const
{
    SETInteger<N> tempProd0 = m_Numer * rR.m_Denom;
    SETInteger<N> tempProd1 = m_Denom * rR.m_Numer;
    if( m_Denom > 0 )
    {
        return (rR.m_Denom > 0 ? tempProd0 > tempProd1 : tempProd0 < 
            tempProd1);
    }
    else
    {
        return (rR.m_Denom > 0 ? tempProd0 < tempProd1 : tempProd0 > 
            tempProd1);
    }
}
//----------------------------------------------------------------------------
template <int N>
SETRational<N> SETRational<N>::operator+(const SETRational& rR) const
{
    SETRational tempSum;
    tempSum.m_Numer = m_Numer*rR.m_Denom + m_Denom*rR.m_Numer;
    tempSum.m_Denom = m_Denom * rR.m_Denom;
    tempSum.EliminatePowersOfTwo();

    return tempSum;
}
//----------------------------------------------------------------------------
template <int N>
SETRational<N> SETRational<N>::operator-(const SETRational& rR) const
{
    SETRational tempDiff;
    tempDiff.m_Numer = m_Numer*rR.m_Denom - m_Denom*rR.m_Numer;
    tempDiff.m_Denom = m_Denom * rR.m_Denom;
    tempDiff.EliminatePowersOfTwo();

    return tempDiff;
}
//----------------------------------------------------------------------------
template <int N>
SETRational<N> SETRational<N>::operator*(const SETRational& rR) const
{
    SETRational tempProd;
    tempProd.m_Numer = m_Numer * rR.m_Numer;
    tempProd.m_Denom = m_Denom * rR.m_Denom;
    tempProd.EliminatePowersOfTwo();

    return tempProd;
}
//----------------------------------------------------------------------------
template <int N>
SETRational<N> SETRational<N>::operator/(const SETRational& rR) const
{
    SETRational tempQuot;
    tempQuot.m_Numer = m_Numer * rR.m_Denom;
    tempQuot.m_Denom = m_Denom * rR.m_Numer;
    tempQuot.EliminatePowersOfTwo();

    return tempQuot;
}
//----------------------------------------------------------------------------
template <int N>
SETRational<N> SETRational<N>::operator-() const
{
    SETRational tempNeg;
    tempNeg.m_Numer = -m_Numer;
    tempNeg.m_Denom = m_Denom;

    return tempNeg;
}
//----------------------------------------------------------------------------
template <int N>
SETRational<N> operator+(const SETInteger<N>& rI,  const SETRational<N>& rR)
{
    SETRational<N> tempSum;
    tempSum.Numer() = rI*rR.Denom() + rR.Numer();
    tempSum.Denom() = rR.Denom();

    return tempSum;
}
//----------------------------------------------------------------------------
template <int N>
SETRational<N> operator-(const SETInteger<N>& rI,  const SETRational<N>& rR)
{
    SETRational<N> tempDiff;
    tempDiff.Numer() = rI*rR.Denom() - rR.Numer();
    tempDiff.Denom() = rR.Denom();

    return tempDiff;
}
//----------------------------------------------------------------------------
template <int N>
SETRational<N> operator*(const SETInteger<N>& rI,  const SETRational<N>& rR)
{
    SETRational<N> tempProd;
    tempProd.Numer() = rI*rR.Numer();
    tempProd.Denom() = rR.Denom();

    return tempProd;
}
//----------------------------------------------------------------------------
template <int N>
SETRational<N> operator/(const SETInteger<N>& rI,  const SETRational<N>& rR)
{
    SETRational<N> tempQuot;
    tempQuot.Numer() = rR.Denom()*rI;
    tempQuot.Denom() = rR.Numer();

    return tempQuot;
}
//----------------------------------------------------------------------------
template <int N>
SETRational<N>& SETRational<N>::operator+=(const SETRational& rR)
{
    *this = *this + rR;
    EliminatePowersOfTwo();

    return *this;
}
//----------------------------------------------------------------------------
template <int N>
SETRational<N>& SETRational<N>::operator-=(const SETRational& rR)
{
    *this = *this - rR;
    EliminatePowersOfTwo();

    return *this;
}
//----------------------------------------------------------------------------
template <int N>
SETRational<N>& SETRational<N>::operator*=(const SETRational& rR)
{
    *this = (*this)*rR;
    EliminatePowersOfTwo();

    return *this;
}
//----------------------------------------------------------------------------
template <int N>
SETRational<N>& SETRational<N>::operator/=(const SETRational& rR)
{
    *this = (*this)/rR;
    EliminatePowersOfTwo();

    return *this;
}
//----------------------------------------------------------------------------
template <int N>
SETRational<N> SETRational<N>::Abs() const
{
    return (*this >= 0 ? *this : -(*this));
}
//----------------------------------------------------------------------------
template <int N>
void SETRational<N>::EliminatePowersOfTwo()
{
    if( (m_Numer.m_asBuffer[0] & 1) > 0
    ||  (m_Denom.m_asBuffer[0] & 1) > 0 )
    {
        // neither term is divisible by 2 (quick out)
        return;
    }

    int iBlock0 = m_Numer.GetTrailingBlock();
    if( iBlock0 == -1 )
    {
        // numerator is zero
        m_Denom = 1;

        return;
    }

    int iBlock1 = m_Denom.GetTrailingBlock();
    SE_ASSERT( iBlock1 >= 0 );  // denominator should never be zero

    int iMinBlock = (iBlock0 < iBlock1 ? iBlock0 : iBlock1);
    int iBit0 = m_Numer.GetTrailingBit(iBlock0);
    int iBit1 = m_Denom.GetTrailingBit(iBlock1);
    int iMinBit = (iBit0 < iBit1 ? iBit0 : iBit1);
    int iShift = 16*iMinBlock + iMinBit;
    m_Numer >>= iShift;
    m_Denom >>= iShift;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// conversions between rational numbers and 'float'
//----------------------------------------------------------------------------
template <int N>
SETRational<N>::SETRational(float fValue)
{
    SETInteger<N> tempOne(1);
    m_Denom = tempOne;
    if( fValue == 0.0f )
    {
        m_Numer = SETInteger<N>(0);

        return;
    }

    // value = sign * 1.mantissa * 2^exponent
    unsigned int uiBits = *(unsigned int*)&fValue;
    unsigned int uiSign = (0x80000000u & uiBits);
    unsigned int uiExponent = ((0x7F800000 & uiBits) >> 23);
    unsigned int uiMantissa = (0x007FFFFF & uiBits);

    // create 1.mantissa
    SETRational tempFraction(1,  2);
    SETInteger<N> tempTwo(2);
    m_Numer = tempOne;
    unsigned int uiMask;
    for( uiMask = 0x00400000; uiMask; uiMask >>= 1,  tempFraction /= tempTwo )
    {
        if( uiMantissa & uiMask )
        {
            *this += tempFraction;
        }
    }

    // multiply by 2^exponent
    SETRational tempMultiplier;
    SETInteger<N> tempPower(2);
    int i,  iDelay = 0;
    if( uiExponent & 0x00000080 )
    {
        tempMultiplier = 2;
        for( i = 0; i <= 6; i++,  uiExponent >>= 1,  iDelay++ )
        {
            if( uiExponent & 1 )
            {
                while( --iDelay >= 0 )
                {
                    tempPower *= tempPower;
                }

                tempMultiplier *= tempPower;
                iDelay = 0;
            }
        }
    }
    else
    {
        tempMultiplier = 1;
        for( i = 0; i <= 6; i++,  uiExponent >>= 1,  iDelay++ )
        {
            if( !(uiExponent & 1) )
            {
                while( --iDelay >= 0 )
                {
                    tempPower *= tempPower;
                }

                tempMultiplier /= tempPower;
                iDelay = 0;
            }
        }
    }

    *this *= tempMultiplier;

    EliminatePowersOfTwo();

    if( uiSign )
    {
        m_Numer = -m_Numer;
    }
}
//----------------------------------------------------------------------------
template <int N>
void SETRational<N>::ConvertTo(float& rfValue) const
{
    SE_ASSERT(m_Denom != 0);

    if( m_Numer == 0 )
    {
        rfValue = 0.0f;
        return;
    }

    unsigned int uiResult;

    // compute the sign of the number
    int iS0 = m_Numer.GetSign(),  iS1 = m_Denom.GetSign();
    int iSign = iS0 * iS1;
    SETInteger<N> tempAbsNumer = iS0 * m_Numer;
    SETInteger<N> tempAbsDenom = iS1 * m_Denom;

    // The rational number is N/D = Q + R/D.  We need to extract 24 bits for
    // 1.mantissa and determine the biased exponent.
    SETInteger<N> tempQuo,  tempRem;
    bool bSuccess = SETInteger<N>::GetDivMod(tempAbsNumer,  tempAbsDenom,  
        tempQuo,  tempRem);
    SE_ASSERT( bSuccess );

    unsigned int uiExponent = 0,  uiMantissa = 0;

    int iBlock = tempQuo.GetLeadingBlock();
    if( iBlock >= 0 )
    {
        // quotient is positive
        if( iBlock >= 8 )
        {
            // quotient larger than the maximum float in magnitude
            if( iSign > 0 )
            {
                uiResult = 0x7F7FFFFFu;  // FLT_MAX
                rfValue = *(float*)&uiResult;
            }
            else
            {
                uiResult = 0xFF7FFFFFu;  // -FLT_MAX
                rfValue = *(float*)&uiResult;
            }
            return;
        }

        if( iBlock == 7 )
        {
            unsigned int uiValue = tempQuo.ToUnsignedInt(iBlock-1,  iBlock);
            if( (uiValue & 0xFFFFFF00u) == 0xFFFFFF00u )
            {
                // quotient larger or equal to the maximum float in magnitude
                if( iSign > 0 )
                {
                    uiResult = 0x7F7FFFFFu;  // FLT_MAX
                    rfValue = *(float*)&uiResult;
                }
                else
                {
                    uiResult = 0xFF7FFFFFu;  // -FLT_MAX
                    rfValue = *(float*)&uiResult;
                }

                return;
            }
        }

        // quotient smaller than the maximum float
        GetPositiveFloat(tempAbsDenom,  tempQuo,  tempRem,  iBlock,  
            uiExponent,  uiMantissa);
        uiResult = uiExponent | uiMantissa;
        if( iSign < 0 )
        {
            uiResult |= 0x80000000;
        }
        rfValue = *(float*)&uiResult;

        return;
    }

    // remainder provides all of 1.mantissa
    for( iBlock = 0; iBlock < 8; iBlock++ )
    {
        // Multiply by 2^{16} to search for 1-bits.  We could do this in one
        // step by using 2^{128},  but this could require an intermediate
        // term of N=16.  The smaller multipliers keep the intermediate terms
        // small.
        tempRem *= 0x10000;
        SETInteger<N> tempNRem;
        bSuccess = SETInteger<N>::GetDivMod(tempRem,  tempAbsDenom,  
            tempQuo,  tempNRem);
        SE_ASSERT( bSuccess );

        tempRem = tempNRem;
        if( tempQuo != 0 )
        {
            break;
        }
    }

    if( iBlock == 8 || (iBlock == 7 && tempQuo.ToUnsignedInt(0) >= 4) )
    {
        // rational number smaller than the minimum floating point number
        if( iSign > 0 )
        {
            uiResult = 0x00800000;  // FLT_MIN
            rfValue = *(float*)&uiResult;
        }
        else
        {
            uiResult = 0x80800000;  // -FLT_MIN
            rfValue = *(float*)&uiResult;
        }
        return;
    }

    // get representation of scaled remainder
    GetPositiveFloat(tempAbsDenom,  tempQuo,  tempRem,  0,  
        uiExponent,  uiMantissa);

    // adjust exponent by how many blocks were used to scale the remainder
    uiExponent >>= 23;
    uiExponent -= 127;
    uiExponent -= 16*(iBlock + 1);
    uiExponent += 127;
    uiExponent <<= 23;

    uiResult = uiExponent | uiMantissa;
    if( iSign < 0 )
    {
        uiResult |= 0x80000000u;
    }

    rfValue = *(float*)&uiResult;
}
//----------------------------------------------------------------------------
template <int N>
void SETRational<N>::GetPositiveFloat(const SETInteger<N>& rDenom, 
    SETInteger<N>& rQuo,  SETInteger<N>& rRem,  int iBlock, 
    unsigned int& ruiExponent,  unsigned int& ruiMantissa)
{
    // SE_ASSERT( rDenom > 0 && rQuo > 0 );

    // quotient smaller than the maximum float
    int iFirstBlockBit = rQuo.GetLeadingBit(iBlock);
    int iFirstBit = iFirstBlockBit + 16*iBlock;
    unsigned int uiMask;
    ruiExponent = ((iFirstBit + 127) << 23);
    iFirstBit--;
    ruiMantissa = 0;
    if( iFirstBit >= 23 )
    {
        // quotient provides all of 1.mantissa
        for( uiMask = 0x00400000; uiMask; uiMask >>= 1, iFirstBit-- )
        {
            if( rQuo.GetBit(iFirstBit) )
            {
                ruiMantissa |= uiMask;
            }
        }
    }
    else
    {
        // quotient contribution to 1.mantissa
        for( uiMask = 0x00400000; iFirstBit >= 0; uiMask >>= 1, iFirstBit-- )
        {
            if( rQuo.GetBit(iFirstBit) )
            {
                ruiMantissa |= uiMask;
            }
        }

        // remainder contribution to 1.mantissa
        for( /**/; uiMask; uiMask >>= 1 )
        {
            rRem *= 2;
            SETInteger<N> tempNRem;
            bool bSuccess = SETInteger<N>::GetDivMod(rRem,  rDenom,  
                rQuo,  tempNRem);
            SE_ASSERT( bSuccess );
            (void)bSuccess;  // avoid compiler warning in release build
            if( rQuo != 0 )
            {
                ruiMantissa |= uiMask;
            }
            rRem = tempNRem;
        }
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// conversions between rational numbers and 'double'
//----------------------------------------------------------------------------
template <int N>
SETRational<N>::SETRational(double dValue)
{
    SETInteger<N> tempOne(1);
    m_Denom = tempOne;
    if( dValue == 0.0 )
    {
        m_Numer = SETInteger<N>(0);

        return;
    }

    // value = sign * 1.mantissa * 2^exponent
    unsigned int* auiBits = (unsigned int*)&dValue;
#ifdef SE_BIG_ENDIAN
    unsigned int uiSave = auiBits[0];
    auiBits[0] = auiBits[1];
    auiBits[1] = uiSave;
#endif
    unsigned int uiSign = (0x80000000u & auiBits[1]);
    unsigned int uiExponent = ((0x7FF00000 & auiBits[1]) >> 20);
    unsigned int uiMantissaHi = (0x000FFFFF & auiBits[1]);
    unsigned int uiMantissaLo = auiBits[0];

    // create 1.mantissa
    SETRational tempFraction(1, 2);
    SETInteger<N> tempTwo(2);
    m_Numer = tempOne;
    unsigned int uiMask;
    for( uiMask = 0x00080000; uiMask; uiMask >>= 1, tempFraction /= tempTwo )
    {
        if( uiMantissaHi & uiMask )
        {
            *this += tempFraction;
        }
    }
    for( uiMask = 0x80000000u; uiMask; uiMask >>= 1, tempFraction /= tempTwo )
    {
        if( uiMantissaLo & uiMask )
        {
            *this += tempFraction;
        }
    }

    // multiply by 2^exponent
    SETRational tempMultiplier;
    SETInteger<N> tempPower(2);
    int i,  iDelay = 0;
    if( uiExponent & 0x400 )
    {
        tempMultiplier = 2;
        for( i = 0; i <= 9; i++,  uiExponent >>= 1,  iDelay++ )
        {
            if( uiExponent & 1 )
            {
                while( --iDelay >= 0 )
                {
                    tempPower *= tempPower;
                }

                tempMultiplier *= tempPower;
                iDelay = 0;
            }
        }
    }
    else
    {
        tempMultiplier = 1;
        for( i = 0; i <= 9; i++,  uiExponent >>= 1,  iDelay++ )
        {
            if( !(uiExponent & 1) )
            {
                while( --iDelay >= 0 )
                {
                    tempPower *= tempPower;
                }

                tempMultiplier /= tempPower;
                iDelay = 0;
            }
        }
    }

    *this *= tempMultiplier;

    EliminatePowersOfTwo();

    if( uiSign )
    {
        m_Numer = -m_Numer;
    }
}
//----------------------------------------------------------------------------
template <int N>
void SETRational<N>::ConvertTo(double& rdValue) const
{
    SE_ASSERT( m_Denom != 0 );

    if( m_Numer == 0 )
    {
        rdValue = 0.0;

        return;
    }

    unsigned int auiResult[2],  uiSave = 0;

    // compute the sign of the number
    int iS0 = m_Numer.GetSign(),  iS1 = m_Denom.GetSign();
    int iSign = iS0 * iS1;
    SETInteger<N> tempAbsNumer = iS0 * m_Numer;
    SETInteger<N> tempAbsDenom = iS1 * m_Denom;

    // The rational number is N/D = Q + R/D.  We need to extract 53 bits for
    // 1.mantissa and determine the biased exponent.
    SETInteger<N> tempQuo,  tempRem;
    bool bSuccess = SETInteger<N>::GetDivMod(tempAbsNumer,  tempAbsDenom,  
        tempQuo,  tempRem);
    SE_ASSERT( bSuccess );

    unsigned int uiExponent = 0,  uiMantissaHi = 0,  uiMantissaLo;

    int iBlock = tempQuo.GetLeadingBlock();
    if( iBlock >= 0 )
    {
        // quotient is positive
        if( iBlock >= 64 )
        {
            // quotient larger than the maximum double in magnitude
            if( iSign > 0 )
            {
                auiResult[0] = 0xFFFFFFFF;  // DBL_MAX
                auiResult[1] = 0x7FEFFFFF;
#ifdef SE_BIG_ENDIAN
                uiSave = auiResult[0];
                auiResult[0] = auiResult[1];
                auiResult[1] = uiSave;
#endif
                rdValue = *(double*)auiResult;
            }
            else
            {
                auiResult[0] = 0xFFFFFFFF;  // -DBL_MAX
                auiResult[1] = 0xFFEFFFFF;
#ifdef SE_BIG_ENDIAN
                uiSave = auiResult[0];
                auiResult[0] = auiResult[1];
                auiResult[1] = uiSave;
#endif
                rdValue = *(double*)auiResult;
            }
            return;
        }

        if( iBlock == 63 )
        {
            unsigned int uiValueHi = tempQuo.ToUnsignedInt(iBlock-1, iBlock);
            unsigned int uiValueLo = tempQuo.ToUnsignedInt(iBlock-3, iBlock-2);
            if( (uiValueHi & 0xFFFFFFFF) == 0xFFFFFFFF
            &&  (uiValueLo & 0xFFFFF800) == 0xFFFFF800 )
            {
                // quotient larger or equal to the maximum float in magnitude
                if( iSign > 0 )
                {
                    auiResult[0] = 0xFFFFFFFF;  // DBL_MAX
                    auiResult[1] = 0x7FEFFFFF;
#ifdef SE_BIG_ENDIAN
                    uiSave = auiResult[0];
                    auiResult[0] = auiResult[1];
                    auiResult[1] = uiSave;
#endif
                    rdValue = *(double*)auiResult;
                }
                else
                {
                    auiResult[0] = 0xFFFFFFFF;  // -DBL_MAX
                    auiResult[1] = 0xFFEFFFFF;
#ifdef SE_BIG_ENDIAN
                    uiSave = auiResult[0];
                    auiResult[0] = auiResult[1];
                    auiResult[1] = uiSave;
#endif
                    rdValue = *(double*)auiResult;
                }

                return;
            }
        }

        // quotient smaller than the maximum float
        GetPositiveDouble(tempAbsDenom, tempQuo, tempRem, iBlock, uiExponent,  
            uiMantissaHi, uiMantissaLo);
        auiResult[1] = uiExponent | uiMantissaHi;
        auiResult[0] = uiMantissaLo;
        if( iSign < 0 )
        {
            auiResult[1] |= 0x80000000u;
        }
#ifdef SE_BIG_ENDIAN
        uiSave = auiResult[0];
        auiResult[0] = auiResult[1];
        auiResult[1] = uiSave;
#endif
        rdValue = *(double*)auiResult;

        return;
    }

    // remainder provides all of 1.mantissa
    for( iBlock = 0; iBlock < 64; iBlock++ )
    {
        // Multiply by 2^{16} to search for 1-bits.  We could do this in one
        // step by using 2^{1024},  but this could require an intermediate
        // term of large N.  The smaller multipliers keep the intermediate
        // terms small.
        tempRem *= 0x10000;
        SETInteger<N> tempNRem;
        bSuccess = SETInteger<N>::GetDivMod(tempRem,  tempAbsDenom,  
            tempQuo,  tempNRem);
        SE_ASSERT( bSuccess );

        tempRem = tempNRem;
        if( tempQuo != 0 )
        {
            break;
        }
    }

    if( iBlock == 64 || (iBlock == 63 && tempQuo.ToUnsignedInt(0) >= 4 ) )
    {
        // rational number smaller than the minimum floating point number
        if( iSign > 0 )
        {
            auiResult[1] = 0x00100000;  // DBL_MIN
            auiResult[0] = 0x00000000;
#ifdef SE_BIG_ENDIAN
            uiSave = auiResult[0];
            auiResult[0] = auiResult[1];
            auiResult[1] = uiSave;
#endif
            rdValue = *(double*)auiResult;
        }
        else
        {
            auiResult[1] = 0x80100000;  // -DBL_MIN
            auiResult[0] = 0x00000000;
#ifdef SE_BIG_ENDIAN
            uiSave = auiResult[0];
            auiResult[0] = auiResult[1];
            auiResult[1] = uiSave;
#endif
            rdValue = *(double*)auiResult;
        }

        return;
    }

    // get representation of scaled remainder
    GetPositiveDouble(tempAbsDenom,  tempQuo,  tempRem,  0,  uiExponent,  
        uiMantissaHi,  uiMantissaLo);

    // adjust exponent by how many blocks were used to scale the remainder
    uiExponent >>= 20;
    uiExponent -= 1023;
    uiExponent -= 16*(iBlock + 1);
    uiExponent += 1023;
    uiExponent <<= 20;

    auiResult[1] = uiExponent | uiMantissaHi;
    auiResult[0] = uiMantissaLo;
    if( iSign < 0 )
    {
        auiResult[1] |= 0x80000000u;
    }
#ifdef SE_BIG_ENDIAN
    uiSave = auiResult[0];
    auiResult[0] = auiResult[1];
    auiResult[1] = uiSave;
#endif
    rdValue = *(double*)auiResult;
}
//----------------------------------------------------------------------------
template <int N>
void SETRational<N>::GetPositiveDouble(const SETInteger<N>& rDenom, 
    SETInteger<N>& rQuo,  SETInteger<N>& rRem,  int iBlock, 
    unsigned int& ruiExponent,  unsigned int& ruiMantissaHi, 
    unsigned int& ruiMantissaLo)
{
    // SE_ASSERT( rDenom > 0 && rQuo > 0 );

    // quotient smaller than the maximum double
    int iFirstBlockBit = rQuo.GetLeadingBit(iBlock);
    int iFirstBit = iFirstBlockBit + 16*iBlock;
    unsigned int uiMask;
    ruiExponent = ((iFirstBit + 1023) << 20);
    iFirstBit--;
    ruiMantissaHi = 0;
    ruiMantissaLo = 0;
    if( iFirstBit >= 52 )
    {
        // quotient provides all of 1.mantissa
        for( uiMask = 0x00080000; uiMask; uiMask >>= 1,  iFirstBit-- )
        {
            if( rQuo.GetBit(iFirstBit) )
            {
                ruiMantissaHi |= uiMask;
            }
        }
        for( uiMask = 0x80000000u; uiMask; uiMask >>= 1,  iFirstBit-- )
        {
            if( rQuo.GetBit(iFirstBit) )
            {
                ruiMantissaLo |= uiMask;
            }
        }
    }
    else
    {
        // quotient contribution to 1.mantissa
        bool bUsingHi = true;
        uiMask = 0x00080000;
        for( /**/; uiMask && iFirstBit >= 0; uiMask >>= 1,  iFirstBit-- )
        {
            if( rQuo.GetBit(iFirstBit) )
            {
                ruiMantissaHi |= uiMask;
            }
        }

        if( iFirstBit > 0 )
        {
            bUsingHi = false;
            uiMask = 0x80000000u;
            for( /**/; iFirstBit >= 0; uiMask >>= 1,  iFirstBit-- )
            {
                if( rQuo.GetBit(iFirstBit) )
                {
                    ruiMantissaLo |= uiMask;
                }
            }
        }

        // remainder contribution to 1.mantissa
        SETInteger<N> tempNRem;
        bool bSuccess;
        if( bUsingHi )
        {
            for( /**/; uiMask; uiMask >>= 1 )
            {
                rRem *= 2;
                bSuccess = SETInteger<N>::GetDivMod(rRem,  rDenom,  
                    rQuo,  tempNRem);
                SE_ASSERT( bSuccess );

                if( rQuo != 0 )
                {
                    ruiMantissaHi |= uiMask;
                }
                rRem = tempNRem;
            }

            for( uiMask = 0x80000000u; uiMask; uiMask >>= 1 )
            {
                rRem *= 2;
                bSuccess = SETInteger<N>::GetDivMod(rRem,  rDenom,  
                    rQuo,  tempNRem);
                SE_ASSERT( bSuccess );

                if( rQuo != 0 )
                {
                    ruiMantissaLo |= uiMask;
                }
                rRem = tempNRem;
            }
        }
        else
        {
            for( /**/; uiMask; uiMask >>= 1 )
            {
                rRem *= 2;
                bSuccess = SETInteger<N>::GetDivMod(rRem,  rDenom,  
                    rQuo,  tempNRem);
                SE_ASSERT( bSuccess );

                if( rQuo != 0 )
                {
                    ruiMantissaLo |= uiMask;
                }
                rRem = tempNRem;
            }
        }
    }
}
//----------------------------------------------------------------------------
