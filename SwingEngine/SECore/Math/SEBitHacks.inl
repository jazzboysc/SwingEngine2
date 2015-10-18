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
inline bool IsOdd(int iValue)
{
    return iValue & 1;
}
//----------------------------------------------------------------------------
inline bool IsEven(int iValue)
{
    return !(iValue & 1);
}
//----------------------------------------------------------------------------
inline bool IsPowerOfTwo(unsigned int uiValue)
{
    return (uiValue > 0) && ((uiValue & (uiValue - 1)) == 0);
}
//----------------------------------------------------------------------------
inline SE_UInt32 RoundUpPowOfTwo(SE_UInt32 uiValue)
{
    uiValue--;
    uiValue |= uiValue >> 1;    uiValue |= uiValue >> 2;
    uiValue |= uiValue >> 4;    uiValue |= uiValue >> 8;
    uiValue |= uiValue >> 16;

    return uiValue + 1;
}

//----------------------------------------------------------------------------
inline unsigned int Log2OfPowerOfTwo(unsigned int uiPowerOfTwo)
{
    unsigned int uiLog2 = (uiPowerOfTwo & 0xAAAAAAAA) != 0;
    uiLog2 |= ((uiPowerOfTwo & 0xFFFF0000) != 0) << 4;
    uiLog2 |= ((uiPowerOfTwo & 0xFF00FF00) != 0) << 3;
    uiLog2 |= ((uiPowerOfTwo & 0xF0F0F0F0) != 0) << 2;
    uiLog2 |= ((uiPowerOfTwo & 0xCCCCCCCC) != 0) << 1;

    return uiLog2;
}
//----------------------------------------------------------------------------
inline void SetBitToOne(unsigned int& rValue, int i)
{
    rValue |= (1 << i);
}
//----------------------------------------------------------------------------
inline void SetBitToZero(unsigned int& rValue, int i)
{
    rValue &= ~(1 << i);
}
//----------------------------------------------------------------------------