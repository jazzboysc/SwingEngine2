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

#include "SEGeometricToolsPCH.h"
#include "SEContBox3.h"
#include "SEApprGaussPointsFit3.h"
#include "SEQuaternion.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEBox3f Swing::ContAABBf(int iCount, const SEVector3f* aPoint)
{
    SEVector3f vec3fMinPoint, vec3fMaxPoint;
    SEVector3f::ComputeExtremes(iCount, aPoint, vec3fMinPoint, vec3fMaxPoint);

    SEBox3f tempBox;
    tempBox.Center = 0.5f * (vec3fMinPoint + vec3fMaxPoint);

    tempBox.Axis[0].X = 1.0f;
    tempBox.Axis[0].Y = 0.0f;
    tempBox.Axis[0].Z = 0.0f;

    tempBox.Axis[1].X = 0.0f;
    tempBox.Axis[1].Y = 1.0f;
    tempBox.Axis[1].Z = 0.0f;

    tempBox.Axis[2].X = 0.0f;
    tempBox.Axis[2].Y = 0.0f;
    tempBox.Axis[2].Z = 1.0f;

    SEVector3f vec3fHalfDiagonal = 0.5f * (vec3fMaxPoint - vec3fMinPoint);
    tempBox.Extent[0] = vec3fHalfDiagonal[0];
    tempBox.Extent[1] = vec3fHalfDiagonal[1];
    tempBox.Extent[2] = vec3fHalfDiagonal[2];

    return tempBox;
}
//----------------------------------------------------------------------------
SEBox3f Swing::ContOBBf(int iCount, const SEVector3f* aPoint)
{
    SEBox3f tempBox = GaussPointsFit3f(iCount, aPoint);

    SEVector3f vec3fDiff = aPoint[0] - tempBox.Center;
    SEVector3f vec3fMinPoint(vec3fDiff.Dot(tempBox.Axis[0]), 
        vec3fDiff.Dot(tempBox.Axis[1]), vec3fDiff.Dot(tempBox.Axis[2]));
    SEVector3f vec3fMaxPoint = vec3fMinPoint;
    for( int i = 1; i < iCount; i++ )
    {
        vec3fDiff = aPoint[i] - tempBox.Center;
        for( int j = 0; j < 3; j++ )
        {
            float fDot = vec3fDiff.Dot(tempBox.Axis[j]);
            if( fDot < vec3fMinPoint[j] )
            {
                vec3fMinPoint[j] = fDot;
            }
            else if( fDot > vec3fMaxPoint[j] )
            {
                vec3fMaxPoint[j] = fDot;
            }
        }
    }

    tempBox.Center +=
        (0.5f*(vec3fMinPoint[0] + vec3fMaxPoint[0]))*tempBox.Axis[0] +
        (0.5f*(vec3fMinPoint[1] + vec3fMaxPoint[1]))*tempBox.Axis[1] +
        (0.5f*(vec3fMinPoint[2] + vec3fMaxPoint[2]))*tempBox.Axis[2];

    tempBox.Extent[0] = 0.5f * (vec3fMaxPoint[0] - vec3fMinPoint[0]);
    tempBox.Extent[1] = 0.5f * (vec3fMaxPoint[1] - vec3fMinPoint[1]);
    tempBox.Extent[2] = 0.5f * (vec3fMaxPoint[2] - vec3fMinPoint[2]);

    return tempBox;
}
//----------------------------------------------------------------------------
bool Swing::IsInBoxf(const SEVector3f& rPoint, const SEBox3f& rBox)
{
    SEVector3f vec3fDiff = rPoint - rBox.Center;
    for( int i = 0; i < 3; i++ )
    {
        float fCoeff = vec3fDiff.Dot(rBox.Axis[i]);
        if( SEMath<float>::FAbs(fCoeff) > rBox.Extent[i] )
        {
            return false;
        }
    }

    return true;
}
//----------------------------------------------------------------------------
SEBox3f Swing::MergeBoxesf(const SEBox3f& rBox0, const SEBox3f& rBox1)
{
    SEBox3f tempBox;

    // 首先初步猜测新box中心.
    // 稍后当传入的box的顶点全部投影完毕后,将更新此值.
    tempBox.Center = 0.5f * (rBox0.Center + rBox1.Center);

    // box的三个轴向量可被视为矩阵的三行,从而构成一个旋转矩阵.
    // 传入的box轴向量被转换为对应其旋转矩阵的四元数.
    // 根据两个四元数,计算其平均四元数,之后规范化为单位长度.
    // 结果四元数就是传入的两个四元数的slerp,其中t为1/2.
    // 然后再把结果四元数转换成与其对应的旋转矩阵.该矩阵的三行将作为新box的三轴.
    SEQuaternionf Q0, Q1;
    Q0.FromRotationMatrix(rBox0.Axis);
    Q1.FromRotationMatrix(rBox1.Axis);
    if( Q0.Dot(Q1) < 0.0f )
    {
        Q1 = -Q1;
    }

    SEQuaternionf Q = Q0 + Q1;
    float fInvLength = SEMath<float>::InvSqrt(Q.Dot(Q));
    Q = fInvLength * Q;
    Q.ToRotationMatrix(tempBox.Axis);

    // 把传入的box的顶点全部投影到merged-box的轴上.
    // 在包含当前box中点C的每个轴D[i]上都有最小投影值pmin[i]和最大投影值pmax[i].
    // 与投影值相对应的轴上端点为C+pmin[i]*D[i]和C+pmax[i]*D[i].
    // 当前C点不一定是每个投影区间的中点.
    // 因此,实际的新box中点将被调整为C':
    //   C' = C + sum_{i=0}^2 0.5*(pmin[i]+pmax[i])*D[i].
    // 新box的extent为:
    //   e[i] = 0.5*(pmax[i]-pmin[i]).

    int i, j;
    float fDot;
    SEVector3f aVertex[8], vec3fDiff;
    SEVector3f vec3fMin = SEVector3f::ZERO;
    SEVector3f vec3fMax = SEVector3f::ZERO;

    rBox0.ComputeVertices(aVertex);
    for( i = 0; i < 8; i++ )
    {
        vec3fDiff = aVertex[i] - tempBox.Center;
        for( j = 0; j < 3; j++ )
        {
            fDot = vec3fDiff.Dot(tempBox.Axis[j]);
            if( fDot > vec3fMax[j] )
            {
                vec3fMax[j] = fDot;
            }
            else if( fDot < vec3fMin[j] )
            {
                vec3fMin[j] = fDot;
            }
        }
    }

    rBox1.ComputeVertices(aVertex);
    for( i = 0; i < 8; i++ )
    {
        vec3fDiff = aVertex[i] - tempBox.Center;
        for( j = 0; j < 3; j++ )
        {
            fDot = vec3fDiff.Dot(tempBox.Axis[j]);
            if( fDot > vec3fMax[j] )
            {
                vec3fMax[j] = fDot;
            }
            else if( fDot < vec3fMin[j] )
            {
                vec3fMin[j] = fDot;
            }
        }
    }

    // [vec3fMin,vec3fMax]是merged box的三轴构成的坐标体系下的AABB.
    // 更新新box的中点C,并根据新中点计算extent.
    for( j = 0; j < 3; j++ )
    {
        tempBox.Center += (0.5f*(vec3fMax[j] + vec3fMin[j])) * tempBox.Axis[j];
        tempBox.Extent[j] = 0.5f*(vec3fMax[j] - vec3fMin[j]);
    }

    return tempBox;
}
//----------------------------------------------------------------------------