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
#include "SEApprGaussPointsFit3.h"
#include "SEEigen.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEBox3f Swing::GaussPointsFit3f(int iCount, const SEVector3f* aPoint)
{
    SEBox3f tempBox(SEVector3f::ZERO, SEVector3f::UNIT_X, SEVector3f::UNIT_Y, 
        SEVector3f::UNIT_Z, 1.0f, 1.0f, 1.0f);

    // 计算顶点的平均中心.
    tempBox.Center = aPoint[0];
    int i;
    for( i = 1; i < iCount; i++ )
    {
        tempBox.Center += aPoint[i];
    }
    float fInvCount = 1.0f / iCount;
    tempBox.Center *= fInvCount;

    // 计算顶点的协方差矩阵.
    float fSumXX = 0.0f, fSumXY = 0.0f, fSumXZ = 0.0f;
    float fSumYY = 0.0f, fSumYZ = 0.0f, fSumZZ = 0.0f;
    for( i = 0; i < iCount; i++ )
    {
        SEVector3f vec3fDiff = aPoint[i] - tempBox.Center;
        fSumXX += vec3fDiff.X * vec3fDiff.X;
        fSumXY += vec3fDiff.X * vec3fDiff.Y;
        fSumXZ += vec3fDiff.X * vec3fDiff.Z;
        fSumYY += vec3fDiff.Y * vec3fDiff.Y;
        fSumYZ += vec3fDiff.Y * vec3fDiff.Z;
        fSumZZ += vec3fDiff.Z * vec3fDiff.Z;
    }

    fSumXX *= fInvCount;
    fSumXY *= fInvCount;
    fSumXZ *= fInvCount;
    fSumYY *= fInvCount;
    fSumYZ *= fInvCount;
    fSumZZ *= fInvCount;

    // 建立eigensolver并求解.
    SEEigen tempES(3);
    tempES(0, 0) = fSumXX;
    tempES(0, 1) = fSumXY;
    tempES(0, 2) = fSumXZ;
    tempES(1, 0) = fSumXY;
    tempES(1, 1) = fSumYY;
    tempES(1, 2) = fSumYZ;
    tempES(2, 0) = fSumXZ;
    tempES(2, 1) = fSumYZ;
    tempES(2, 2) = fSumZZ;
    tempES.IncrSortEigenStuff3();

    for( i = 0; i < 3; i++ )
    {
        tempBox.Extent[i] = tempES.GetEigenvalue(i);
        tempES.GetEigenvector(i, tempBox.Axis[i]);
    }

    return tempBox;
}
//----------------------------------------------------------------------------
