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

// Bit hacks are available at
//     http://graphics.stanford.edu/~seander/bithacks.html

#ifndef Swing_BitHacks_H
#define Swing_BitHacks_H

#include "SEPlatforms.h"

// 一些位操作函数和位操作实现的数学函数.
namespace Swing
{

// 判断iValue是否为奇数.
inline bool IsOdd(int iValue);

// 判断iValue是否为偶数.
inline bool IsEven(int iValue);

// 判断iValue是否为2^n.
inline bool IsPowerOfTwo(int iValue);

// 向上获取最接近uiValue的是2^n的数.
inline SE_UInt32 RoundUpPowOfTwo(SE_UInt32 uiValue);

// uiPowerOfTwo = 2^n,求n.
inline unsigned int Log2OfPowerOfTwo(unsigned int uiPowerOfTwo);

// 将指定位设置为1.
inline void SetBitToOne(unsigned int& rValue, int i);

// 将指定位设置为0.
inline void SetBitToZero(unsigned int& rValue, int i);

#include "SEBitHacks.inl"

}

#endif
