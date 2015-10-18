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

#include "SECorePCH.h"
#include "SETransformation.h"

using namespace Swing;

const SETransformation SETransformation::IDENTITY;

//----------------------------------------------------------------------------
SETransformation::SETransformation()
    :
    m_Matrix(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
    m_Translate(0.0f, 0.0f, 0.0f),
    m_Scale(1.0f, 1.0f, 1.0f)
{
    m_bIsIdentity = true;
    m_bIsSRMatrix = true;
    m_bIsUniformScale = true;
}
//----------------------------------------------------------------------------
SETransformation::~SETransformation()
{
}
//----------------------------------------------------------------------------
void SETransformation::Identity()
{
    m_Matrix = SEMatrix3f::IDENTITY;
    m_Translate = SEVector3f::ZERO;
    m_Scale = SEVector3f(1.0f, 1.0f, 1.0f);
    m_bIsIdentity = true;
    m_bIsSRMatrix = true;
    m_bIsUniformScale = true;
}
//----------------------------------------------------------------------------
void SETransformation::UnitScale()
{
    SE_ASSERT( m_bIsSRMatrix );

    m_Scale = SEVector3f(1.0f, 1.0f, 1.0f);
    m_bIsUniformScale = true;
}
//----------------------------------------------------------------------------
float SETransformation::GetNorm() const
{
    if( m_bIsSRMatrix )
    {
        float fMax = SEMathf::FAbs(m_Scale.X);
        if( SEMathf::FAbs(m_Scale.Y) > fMax )
            fMax = SEMathf::FAbs(m_Scale.Y);
        if( SEMathf::FAbs(m_Scale.Z) > fMax )
            fMax = SEMathf::FAbs(m_Scale.Z);
        return fMax;
    }

    float fMaxColSum =
        SEMathf::FAbs(m_Matrix[0][0]) +
        SEMathf::FAbs(m_Matrix[1][0]) +
        SEMathf::FAbs(m_Matrix[2][0]);

    float fColSum =
        SEMathf::FAbs(m_Matrix[0][1]) +
        SEMathf::FAbs(m_Matrix[1][1]) +
        SEMathf::FAbs(m_Matrix[2][1]);

    if( fColSum > fMaxColSum )
        fMaxColSum = fColSum;

    fColSum =
        SEMathf::FAbs(m_Matrix[0][2]) +
        SEMathf::FAbs(m_Matrix[1][2]) +
        SEMathf::FAbs(m_Matrix[2][2]);

    if( fColSum > fMaxColSum )
        fMaxColSum = fColSum;

    return fMaxColSum;
}
//----------------------------------------------------------------------------
void SETransformation::SetRotate(const SEMatrix3f& rRotate)
{
    m_Matrix = rRotate;
    m_bIsIdentity = false;
    m_bIsSRMatrix = true;
}
//----------------------------------------------------------------------------
void SETransformation::SetTranslate(const SEVector3f& rTranslate)
{
    m_Translate = rTranslate;
    m_bIsIdentity = false;
}
//----------------------------------------------------------------------------
void SETransformation::SetScale(const SEVector3f& rScale)
{
    SE_ASSERT( m_bIsSRMatrix && rScale.X != 0.0f && rScale.Y != 0.0f &&
        rScale.Z != 0.0f );

    m_Scale = rScale;
    m_bIsIdentity = false;
    m_bIsUniformScale = false;
}
//----------------------------------------------------------------------------
void SETransformation::SetUniformScale(float fScale)
{
    SE_ASSERT( m_bIsSRMatrix && fScale != 0.0f );

    m_Scale = SEVector3f(fScale, fScale, fScale);
    m_bIsIdentity = false;
    m_bIsUniformScale = true;
}
//----------------------------------------------------------------------------
void SETransformation::SetMatrix(const SEMatrix3f& rMatrix)
{
    m_Matrix = rMatrix;
    m_bIsIdentity = false;
    m_bIsSRMatrix = false;
    m_bIsUniformScale = false;
}
//----------------------------------------------------------------------------
void SETransformation::ApplyForwardToPoint(const SEVector3f& rInput, 
    SEVector3f& rOutput) const
{
    if( m_bIsIdentity )
    {
        // Y = X
        rOutput = rInput;
        return;
    }

    if( m_bIsSRMatrix )
    {
        // Y = X*S*R + T
		rOutput.X = m_Scale.X * rInput.X;
		rOutput.Y = m_Scale.Y * rInput.Y;
		rOutput.Z = m_Scale.Z * rInput.Z;
        rOutput = rOutput*m_Matrix + m_Translate;
        return;
    }

    // Y = X*M + T
    rOutput = rInput*m_Matrix + m_Translate;
}
//----------------------------------------------------------------------------
void SETransformation::ApplyForwardToPoint(int iCount, 
    const SEVector3f* aInput, SEVector3f* aOutput) const
{
    if( m_bIsIdentity )
    {
        // Y = X
        memcpy(aOutput, aInput, iCount * sizeof(SEVector3f));
    }
    else
    {
        int i;
        if( m_bIsSRMatrix )
        {
            // Y = X*S*R + T
            for( i = 0; i < iCount; ++i )
            {
                aOutput[i].X = m_Scale.X * aInput[i].X;
                aOutput[i].Y = m_Scale.Y * aInput[i].Y;
                aOutput[i].Z = m_Scale.Z * aInput[i].Z;
                aOutput[i] = aOutput[i]*m_Matrix + m_Translate;
            }
        }
        else
        {
            // Y = X*M + T
            for( i = 0; i < iCount; ++i )
            {
                aOutput[i] = aInput[i]*m_Matrix + m_Translate;
            }
        }
    }
}
//----------------------------------------------------------------------------
void SETransformation::ApplyInverseToPoint(const SEVector3f& rInput, 
    SEVector3f& rOutput) const
{
    if( m_bIsIdentity )
    {
        // X = Y
        rOutput = rInput;
        return;
    }

    rOutput = rInput - m_Translate;
    if( m_bIsSRMatrix )
    {
        // X = (Y-T)*R^T*S^{-1}
        rOutput = m_Matrix * rOutput;
        if( m_bIsUniformScale )
        {
            rOutput /= GetUniformScale();
        }
        else
        {
            // 获取非统一缩放变换的逆变换如下
            //   rOutput.X /= m_Scale.X;
            //   rOutput.Y /= m_Scale.Y;
            //   rOutput.Z /= m_Scale.Z;
            // 使用了一个除法替换的技巧,
            // 3次除法被替换成1次除法和9次乘法,
            // 对于Intel Pentium级别CPU来说,
            // 除法将占用39个时钟周期,乘法只需要3个时钟周期.
            float fSXY = m_Scale.X * m_Scale.Y;
            float fSXYZ = fSXY * m_Scale.Z;
            float fInvSXYZ = 1.0f / fSXYZ;
            float fInvSXY = fInvSXYZ * m_Scale.Z;
            float fInvXScale = fInvSXY * m_Scale.Y;
            float fInvYScale = fInvSXY * m_Scale.X;
            float fInvZScale = fInvSXYZ * fSXY;
            rOutput.X *= fInvXScale;
            rOutput.Y *= fInvYScale;
            rOutput.Z *= fInvZScale;
        }
    }
    else
    {
        // X = (Y-T)*M^{-1}
        SEMatrix3f InvMat;
		m_Matrix.GetInverse(InvMat);
        rOutput = rOutput * InvMat;
    }
}
//----------------------------------------------------------------------------
void SETransformation::ApplyInverseToPoint(int iCount, 
    const SEVector3f* aInput, SEVector3f* aOutput) const
{
    if( m_bIsIdentity )
    {
        // X = Y
        memcpy(aOutput, aInput, iCount * sizeof(SEVector3f));
        return;
    }

    SEVector3f tempVec;
    int i;
    if( m_bIsSRMatrix )
    {
        // X = (Y-T)*R^T*S^{-1}
        if( m_bIsUniformScale )
        {
            float fInvScale = 1.0f / GetUniformScale();
            for( i = 0; i < iCount; ++i )
            {
                tempVec = aInput[i] - m_Translate;
				aOutput[i] = fInvScale * (m_Matrix * tempVec);
            }
        }
        else
        {
            float fSXY = m_Scale.X * m_Scale.Y;
            float fSXYZ = fSXY * m_Scale.Z;
            float fInvSXYZ = 1.0f / fSXYZ;
            float fInvSXY = fInvSXYZ * m_Scale.Z;
            float fInvXScale = fInvSXY * m_Scale.Y;
            float fInvYScale = fInvSXY * m_Scale.X;
            float fInvZScale = fInvSXYZ * fSXY;
            for( i = 0; i < iCount; ++i )
            {
                tempVec = aInput[i] - m_Translate;
                aOutput[i] = m_Matrix * tempVec;
                aOutput[i].X *= fInvXScale;
                aOutput[i].Y *= fInvYScale;
                aOutput[i].Z *= fInvZScale;
            }
        }
    }
    else
    {
        // X = (Y-T)*M^{-1}
        SEMatrix3f InvMat; 
        m_Matrix.GetInverse(InvMat);
        for( i = 0; i < iCount; ++i )
        {
            tempVec = aInput[i] - m_Translate;
            aOutput[i] = tempVec * InvMat;
        }
    }
}
//----------------------------------------------------------------------------
void SETransformation::ApplyForwardToVector(const SEVector3f& rInput, 
    SEVector3f& rOutput) const
{
    if( m_bIsIdentity )
    {
        // Y = X
        rOutput = rInput;
        return;
    }

    if( m_bIsSRMatrix )
    {
        // Y = X*S*R
		rOutput.X = m_Scale.X * rInput.X;
		rOutput.Y = m_Scale.Y * rInput.Y;
		rOutput.Z = m_Scale.Z * rInput.Z;
        rOutput = rOutput*m_Matrix;
        return;
    }

    // Y = X*M
    rOutput = rInput*m_Matrix;
}
//----------------------------------------------------------------------------
void SETransformation::ApplyInverseToVector(const SEVector3f& rInput, 
    SEVector3f& rOutput) const
{
    if( m_bIsIdentity )
    {
        // X = Y
        rOutput = rInput;
        return;
    }

    if( m_bIsSRMatrix )
    {
        // X = Y*R^T*S^{-1}
        rOutput = m_Matrix * rInput;
        if( m_bIsUniformScale )
        {
            rOutput /= GetUniformScale();
        }
        else
        {
            float fSXY = m_Scale.X * m_Scale.Y;
            float fSXYZ = fSXY * m_Scale.Z;
            float fInvSXYZ = 1.0f / fSXYZ;
            float fInvSXY = fInvSXYZ * m_Scale.Z;
            float fInvXScale = fInvSXY * m_Scale.Y;
            float fInvYScale = fInvSXY * m_Scale.X;
            float fInvZScale = fInvSXYZ * fSXY;
            rOutput.X *= fInvXScale;
            rOutput.Y *= fInvYScale;
            rOutput.Z *= fInvZScale;
        }
    }
    else
    {
        // X = Y*M^{-1}
        SEMatrix3f InvMat;
        m_Matrix.GetInverse(InvMat);
        rOutput = rInput * InvMat;
    }
}
//----------------------------------------------------------------------------
void SETransformation::ApplyForwardToNormal(const SEVector3f& rInput, 
    SEVector3f& rOutput) const
{
    if( m_bIsIdentity )
    {
        // X = Y
        rOutput = rInput;
        return;
    }

    if( m_bIsSRMatrix )
    {
        // 因为Y = X*(M^{-1})^t
        // 因此Y = X*(R^{-1}*S^{-1})^t
        // 因此Y = X*S^{-1}*R

        if( m_bIsUniformScale )
        {
            rOutput = rInput / GetUniformScale();
        }
        else
        {
            float fSXY = m_Scale.X * m_Scale.Y;
            float fSXYZ = fSXY * m_Scale.Z;
            float fInvSXYZ = 1.0f / fSXYZ;
            float fInvSXY = fInvSXYZ * m_Scale.Z;
            float fInvXScale = fInvSXY * m_Scale.Y;
            float fInvYScale = fInvSXY * m_Scale.X;
            float fInvZScale = fInvSXYZ * fSXY;
            rOutput.X = rInput.X * fInvXScale;
            rOutput.Y = rInput.Y * fInvYScale;
            rOutput.Z = rInput.Z * fInvZScale;
        }
        rOutput = rOutput * m_Matrix;
        return;
    }

    // Y = X*(M^{-1})^t
    SEMatrix3f InvMat;
    m_Matrix.GetInverse(InvMat);
    rOutput = InvMat * rInput;
}
//----------------------------------------------------------------------------
void SETransformation::ApplyInverseToNormal(const SEVector3f& rInput, 
    SEVector3f& rOutput) const
{
    if( m_bIsIdentity )
    {
        // X = Y
        rOutput = rInput;
        return;
    }

    if( m_bIsSRMatrix )
    {
        // 因为Y = X*M^t
        // 因此Y = X*R^t*S
        rOutput = m_Matrix * rInput;
		rOutput.X = m_Scale.X * rInput.X;
		rOutput.Y = m_Scale.Y * rInput.Y;
		rOutput.Z = m_Scale.Z * rInput.Z;
        return;
    }

    // Y = X*M^t
    rOutput = m_Matrix * rInput;
}
//----------------------------------------------------------------------------
void SETransformation::ApplyForwardToRay(const SERay3f& rInput, 
    SERay3f& rOutput) const
{
    ApplyForwardToPoint(rInput.Origin, rOutput.Origin);
    ApplyForwardToVector(rInput.Direction, rOutput.Direction);
    rOutput.MinT = rInput.MinT;
    rOutput.MaxT = rInput.MaxT;
    rOutput.Depth = rInput.Depth;
    rOutput.Time = rInput.Time;
}
//----------------------------------------------------------------------------
void SETransformation::ApplyInverseToRay(const SERay3f& rInput, 
    SERay3f& rOutput) const
{
    ApplyInverseToPoint(rInput.Origin, rOutput.Origin);
    ApplyInverseToVector(rInput.Direction, rOutput.Direction);
    rOutput.MinT = rInput.MinT;
    rOutput.MaxT = rInput.MaxT;
    rOutput.Depth = rInput.Depth;
    rOutput.Time = rInput.Time;
}
//----------------------------------------------------------------------------
void SETransformation::ApplyForwardToRayDifferential(
    const SERayDifferential& rInput, SERayDifferential& rOutput) const
{
    ApplyForwardToRay(rInput, rOutput);
    rOutput.HasDifferentials = rInput.HasDifferentials;
    ApplyForwardToPoint(rInput.RxOrigin, rOutput.RxOrigin);
    ApplyForwardToPoint(rInput.RyOrigin, rOutput.RyOrigin);
    ApplyForwardToVector(rInput.RxDirection, rOutput.RxDirection);
    ApplyForwardToVector(rInput.RyDirection, rOutput.RyDirection);
}
//----------------------------------------------------------------------------
void SETransformation::ApplyInverseToRayDifferential(
    const SERayDifferential& rInput, SERayDifferential& rOutput) const
{
    ApplyInverseToRay(rInput, rOutput);
    rOutput.HasDifferentials = rInput.HasDifferentials;
    ApplyInverseToPoint(rInput.RxOrigin, rOutput.RxOrigin);
    ApplyInverseToPoint(rInput.RyOrigin, rOutput.RyOrigin);
    ApplyInverseToVector(rInput.RxDirection, rOutput.RxDirection);
    ApplyInverseToVector(rInput.RyDirection, rOutput.RyDirection);
}
//----------------------------------------------------------------------------
void SETransformation::ApplyForwardToAABB(const SEAxisAlignedBox3f& rInput, 
    SEAxisAlignedBox3f& rOutput) const
{
    SEVector3f tempP;

    ApplyForwardToPoint(
        SEVector3f(rInput.Min[0], rInput.Min[1], rInput.Min[2]), tempP);
    rOutput = tempP;
    
    ApplyForwardToPoint(
        SEVector3f(rInput.Max[0], rInput.Min[1], rInput.Min[2]), tempP);
    rOutput = Union(rOutput, tempP);

    ApplyForwardToPoint(
        SEVector3f(rInput.Min[0], rInput.Max[1], rInput.Min[2]), tempP);
    rOutput = Union(rOutput, tempP);

    ApplyForwardToPoint(
        SEVector3f(rInput.Min[0], rInput.Min[1], rInput.Max[2]), tempP);
    rOutput = Union(rOutput, tempP);

    ApplyForwardToPoint(
        SEVector3f(rInput.Min[0], rInput.Max[1], rInput.Max[2]), tempP);
    rOutput = Union(rOutput, tempP);

    ApplyForwardToPoint(
        SEVector3f(rInput.Max[0], rInput.Max[1], rInput.Min[2]), tempP);
    rOutput = Union(rOutput, tempP);

    ApplyForwardToPoint(
        SEVector3f(rInput.Max[0], rInput.Min[1], rInput.Max[2]), tempP);
    rOutput = Union(rOutput, tempP);

    ApplyForwardToPoint(
        SEVector3f(rInput.Max[0], rInput.Max[1], rInput.Max[2]), tempP);
    rOutput = Union(rOutput, tempP);
}
//----------------------------------------------------------------------------
void SETransformation::ApplyInverseToAABB(const SEAxisAlignedBox3f& rInput, 
    SEAxisAlignedBox3f& rOutput) const
{
    SEVector3f tempP;

    ApplyInverseToPoint(
        SEVector3f(rInput.Min[0], rInput.Min[1], rInput.Min[2]), tempP);
    rOutput = tempP;
    
    ApplyInverseToPoint(
        SEVector3f(rInput.Max[0], rInput.Min[1], rInput.Min[2]), tempP);
    rOutput = Union(rOutput, tempP);

    ApplyInverseToPoint(
        SEVector3f(rInput.Min[0], rInput.Max[1], rInput.Min[2]), tempP);
    rOutput = Union(rOutput, tempP);

    ApplyInverseToPoint(
        SEVector3f(rInput.Min[0], rInput.Min[1], rInput.Max[2]), tempP);
    rOutput = Union(rOutput, tempP);

    ApplyInverseToPoint(
        SEVector3f(rInput.Min[0], rInput.Max[1], rInput.Max[2]), tempP);
    rOutput = Union(rOutput, tempP);

    ApplyInverseToPoint(
        SEVector3f(rInput.Max[0], rInput.Max[1], rInput.Min[2]), tempP);
    rOutput = Union(rOutput, tempP);

    ApplyInverseToPoint(
        SEVector3f(rInput.Max[0], rInput.Min[1], rInput.Max[2]), tempP);
    rOutput = Union(rOutput, tempP);

    ApplyInverseToPoint(
        SEVector3f(rInput.Max[0], rInput.Max[1], rInput.Max[2]), tempP);
    rOutput = Union(rOutput, tempP);
}
//----------------------------------------------------------------------------
void SETransformation::ApplyForwardToPlane(const SEPlane3f& rInput, SEPlane3f& 
    rOutput) const
{
    if( m_bIsIdentity )
    {
        rOutput = rInput;
        return;
    }

    if( m_bIsSRMatrix )
    {
        // 对于SRT情况
        // 设X表示模型空间平面上任意一点,Y = X*S*R + T表示世界空间平面上任意
        // 一点,S是世界缩放变换,R是世界旋转变换,T是世界平移变换.
        // 世界空间到模型空间的逆变换为X = (Y-T)*R^T*S^{-1}.
        // 模型空间的平面方程为dot(N0,X) = C0.将X替换为Y的方程,则可推导出
        // 世界空间的平面方程为dot(N1,Y) = C1.
        // 其中N1 = N0*S^{-1}*R,C1 = C0 + dot(N1,T).如果S不是单位矩阵,
        // 则N1将不是单位向量,因此需要规范化N1并且调整C1:N1' = N1/|N1|,
        // C1' = C1/|N1|.
        if( m_bIsUniformScale )
        {
            // N1 = N0*S^{-1}*R,由于统一缩放变换不会改变向量的空间方向姿态,
            // 因此只需对向量施加旋转变换
            // 此时上式等价为N1 = N0*R
            rOutput.Normal = rInput.Normal * m_Matrix;
            // C1 = (C0 / |N0*S^{-1}*R|) + dot(N1,T) (1)
            // 由于旋转变换不会改变向量长度,因此|N0*S^{-1}*R| = |N0*S^{-1}|,
            // 又因为是统一缩放变换且N0是单位向量,
            // 因此|N0*S^{-1}| = 1.0f / GetUniformScale().
            // 因此式(1)等价为C1 = C0 * GetUniformScale() + dot(N1,T).
            rOutput.Constant = GetUniformScale() * rInput.Constant +
                rOutput.Normal.Dot(m_Translate);

            return;
        }

        rOutput.Normal = rInput.Normal;

        // 如果是非统一缩放变换,则向量的空间方向姿态将会受到影响,
        // 因此需施加非统一缩放变换的逆变换与旋转变换
        float fSXY = m_Scale.X * m_Scale.Y;
        float fSXYZ = fSXY * m_Scale.Z;
        float fInvSXYZ = 1.0f / fSXYZ;
        float fInvSXY = fInvSXYZ * m_Scale.Z;
        float fInvXScale = fInvSXY * m_Scale.Y;
        float fInvYScale = fInvSXY * m_Scale.X;
        float fInvZScale = fInvSXYZ * fSXY;
        rOutput.Normal.X *= fInvXScale;
        rOutput.Normal.Y *= fInvYScale;
        rOutput.Normal.Z *= fInvZScale;
        rOutput.Normal = rOutput.Normal * m_Matrix;
    }
    else
    {
        // 对于一般MT情况,
        // 设X表示模型空间平面上任意一点,
        // Y = X*M + T表示世界空间平面上任意一点,M是任意综合变换,
        // 世界空间到模型空间的逆变换为X = (Y-T)*M^{-1}.
        // 模型空间的平面方程为dot(N0,X) = C0.将X替换为Y的方程,则可推导出
        // 世界空间的平面方程为dot(N1,Y) = C1.
        // 推导过程:
        // (1) dot(N0,X) = C0 => dot(N0,(Y-T)*M^{-1}) = C0
        // (2) dot(N0,(Y-T)*M^{-1}) = C0 => 
        //     dot(N0,Y*M^{-1}) - dot(N0,T*M^{-1}) = C0
        // (3) dot(N0,Y*M^{-1}) - dot(N0,T*M^{-1}) = C0 =>
        //     dot(N0,(M^{-1})^T*Y) = C0 + dot(N0,T*M^{-1})
        // (4) dot(N0,(M^{-1})^T*Y) = C0 + dot(N0,T*M^{-1}) =>
        //     N0*(M^{-1})^T*Y = C0 + N0*M^{-1}^T*T
        // 此时N0*(M^{-1})^T就是世界空间平面的法线,
        // 对等式(4)两边同时除以该法线的模,得到最终N1为单位向量
        // 因此N1 = N0*M^{-T} / |N0*M^{-1}| (-T表示求逆后转置),
        // C1 = (C0 / |N0*M^{-1}|) + dot(N1,T).
        // 注意DirectX中的D3DXPlaneTransform*函数也是基于此数学原理,
        // 因此D3DXPlaneTransform*函数要求传入的综合变换矩阵为:
        // 该矩阵的逆矩阵的转置矩阵.
        SEMatrix3f InvMat;
        m_Matrix.GetInverse(InvMat);
        rOutput.Normal = InvMat * rInput.Normal;
    }

    float fInvLength = 1.0f / rOutput.Normal.GetLength();
    rOutput.Normal *= fInvLength;
    rOutput.Constant = fInvLength * rInput.Constant +
        rOutput.Normal.Dot(m_Translate);
}
//----------------------------------------------------------------------------
void SETransformation::Product(const SETransformation& rLhsTrans,
    const SETransformation& rRhsTrans)
{
    if( rLhsTrans.IsIdentity() )
    {
        *this = rRhsTrans;
        return;
    }

    if( rRhsTrans.IsIdentity() )
    {
        *this = rLhsTrans;
        return;
    }

    // 如果rLhsTrans和rRhsTrans都是SRT形式,结果变换才能是SRT形式.
    if( rLhsTrans.m_bIsSRMatrix && rRhsTrans.m_bIsSRMatrix )
    {
        if( rRhsTrans.m_bIsUniformScale )
        {
            SetRotate(rLhsTrans.m_Matrix * rRhsTrans.m_Matrix);

            SetTranslate(rRhsTrans.GetUniformScale()*(rLhsTrans.m_Translate *
                rRhsTrans.m_Matrix) + rRhsTrans.m_Translate);

            if( rLhsTrans.IsUniformScale() )
            {
                SetUniformScale(rLhsTrans.GetUniformScale() *
                    rRhsTrans.GetUniformScale());
            }
            else
            {
                SetScale(rRhsTrans.GetUniformScale() * rLhsTrans.GetScale());
            }

            return;
        }
    }

    // 其他情况下结果变换都是MT形式.
    SEMatrix3f MatA, MatB;

    if( rLhsTrans.m_bIsSRMatrix )
    {
        rLhsTrans.m_Matrix.GetDiagonalTimes(rLhsTrans.m_Scale, MatA);
    }
    else
    {
        MatA = rLhsTrans.m_Matrix;
    }

    if( rRhsTrans.m_bIsSRMatrix )
    {
        rRhsTrans.m_Matrix.GetDiagonalTimes(rRhsTrans.m_Scale, MatB);
    }
    else
    {
        MatB = rRhsTrans.m_Matrix;
    }

    SetMatrix(MatA * MatB);
    SetTranslate(rLhsTrans.m_Translate*MatB + rRhsTrans.m_Translate);
}
//----------------------------------------------------------------------------
void SETransformation::GetInverse(SETransformation& rInvTrans)
{
    if( m_bIsIdentity )
    {
        rInvTrans = *this;
        return;
    }

    if( m_bIsSRMatrix )
    {
        if( m_bIsUniformScale )
        {
            SEMatrix3f TranMat;
            GetRotate().GetTranspose(TranMat);
            rInvTrans.m_Matrix = TranMat / GetUniformScale();
        }
        else
        {
            m_Matrix.GetDiagonalTimes(m_Scale, rInvTrans.m_Matrix);
			rInvTrans.m_Matrix.Inverse();
        }
    }
    else
    {
        m_Matrix.GetInverse(rInvTrans.m_Matrix);
    }

    rInvTrans.m_Translate = -(m_Translate * rInvTrans.m_Matrix);
    rInvTrans.m_bIsIdentity = false;
    rInvTrans.m_bIsSRMatrix = false;
    rInvTrans.m_bIsUniformScale = false;
}
//----------------------------------------------------------------------------
void SETransformation::GetHomogeneous(SEMatrix4f& rHMatrix) const
{
    if( m_bIsSRMatrix )
    {
        rHMatrix[0][0] = m_Scale[0]*m_Matrix[0][0];
        rHMatrix[0][1] = m_Scale[0]*m_Matrix[0][1];
        rHMatrix[0][2] = m_Scale[0]*m_Matrix[0][2];
        rHMatrix[0][3] = 0.0f;
        rHMatrix[1][0] = m_Scale[1]*m_Matrix[1][0];
        rHMatrix[1][1] = m_Scale[1]*m_Matrix[1][1];
        rHMatrix[1][2] = m_Scale[1]*m_Matrix[1][2];
        rHMatrix[1][3] = 0.0f;
        rHMatrix[2][0] = m_Scale[2]*m_Matrix[2][0];
        rHMatrix[2][1] = m_Scale[2]*m_Matrix[2][1];
        rHMatrix[2][2] = m_Scale[2]*m_Matrix[2][2];
        rHMatrix[2][3] = 0.0f;
    }
    else
    {
        rHMatrix[0][0] = m_Matrix[0][0];
        rHMatrix[0][1] = m_Matrix[0][1];
        rHMatrix[0][2] = m_Matrix[0][2];
        rHMatrix[0][3] = 0.0f;
        rHMatrix[1][0] = m_Matrix[1][0];
        rHMatrix[1][1] = m_Matrix[1][1];
        rHMatrix[1][2] = m_Matrix[1][2];
        rHMatrix[1][3] = 0.0f;
        rHMatrix[2][0] = m_Matrix[2][0];
        rHMatrix[2][1] = m_Matrix[2][1];
        rHMatrix[2][2] = m_Matrix[2][2];
        rHMatrix[2][3] = 0.0f;
    }

    rHMatrix[3][0] = m_Translate[0];
    rHMatrix[3][1] = m_Translate[1];
    rHMatrix[3][2] = m_Translate[2];
    rHMatrix[3][3] = 1.0f;
}
//----------------------------------------------------------------------------
