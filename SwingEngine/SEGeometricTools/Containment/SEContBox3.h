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

#ifndef Swing_ContBox3_H
#define Swing_ContBox3_H

#include "SEGeometricToolsLIB.h"
#include "SEBox3.h"

namespace Swing
{

// 根据输入的若干顶点,计算其最小AABB.
SE_GEOMETRICTOOLS_API SEBox3f ContAABBf(int iCount, const SEVector3f* aPoint);

// 根据输入的若干顶点,计算其OBB,OBB中心是顶点平均中心,
// OBB轴是顶点协方差矩阵的特征向量.
SE_GEOMETRICTOOLS_API SEBox3f ContOBBf(int iCount, const SEVector3f* aPoint);

// 测试指定顶点是否包含于盒子内.
SE_GEOMETRICTOOLS_API bool IsInBoxf(const SEVector3f& rPoint, const SEBox3f& rBox);

// 计算包含两个OBB的新OBB,该OBB不能保证最小.
SE_GEOMETRICTOOLS_API SEBox3f MergeBoxesf(const SEBox3f& rBox0, const SEBox3f& rBox1);

}

#endif