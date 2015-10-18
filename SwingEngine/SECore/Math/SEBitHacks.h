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

// һЩλ����������λ����ʵ�ֵ���ѧ����.
namespace Swing
{

// �ж�iValue�Ƿ�Ϊ����.
inline bool IsOdd(int iValue);

// �ж�iValue�Ƿ�Ϊż��.
inline bool IsEven(int iValue);

// �ж�iValue�Ƿ�Ϊ2^n.
inline bool IsPowerOfTwo(int iValue);

// ���ϻ�ȡ��ӽ�uiValue����2^n����.
inline SE_UInt32 RoundUpPowOfTwo(SE_UInt32 uiValue);

// uiPowerOfTwo = 2^n,��n.
inline unsigned int Log2OfPowerOfTwo(unsigned int uiPowerOfTwo);

// ��ָ��λ����Ϊ1.
inline void SetBitToOne(unsigned int& rValue, int i);

// ��ָ��λ����Ϊ0.
inline void SetBitToZero(unsigned int& rValue, int i);

#include "SEBitHacks.inl"

}

#endif
