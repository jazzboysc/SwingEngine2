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

#ifndef Swing_ApprGaussPointsFit3_H
#define Swing_ApprGaussPointsFit3_H

#include "SEGeometricToolsLIB.h"
#include "SEBox3.h"

namespace Swing
{

// 用Gaussian distribution创建box,
// 中心点是平均顶点中心,
// 轴是规范正交基,由顶点协方差矩阵的特征向量构成,
// 特征向量对应的特征值用作Box的半长,半宽,半高,
// 特征向量顺序与特征值顺序对应,特征值由小到大排列
SE_GEOMETRICTOOLS_API SEBox3f GaussPointsFit3f(int iCount, const SEVector3f* aPoint);

}

#endif
