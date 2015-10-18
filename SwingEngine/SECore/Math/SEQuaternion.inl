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
inline float SEQuaternionf::GetLength() const
{
    return SEMath<float>::Sqrt(m_fData[0]*m_fData[0] +
                             m_fData[1]*m_fData[1] +
                             m_fData[2]*m_fData[2] +
                             m_fData[3]*m_fData[3]);
}
//----------------------------------------------------------------------------
inline float SEQuaternionf::GetSquaredLength() const
{
    return
        m_fData[0]*m_fData[0] +
        m_fData[1]*m_fData[1] +
        m_fData[2]*m_fData[2] +
        m_fData[3]*m_fData[3];
}
//----------------------------------------------------------------------------
inline float SEQuaternionf::Dot(const SEQuaternionf& rQ) const
{
    return 
        m_fData[0]*rQ.m_fData[0] +
        m_fData[1]*rQ.m_fData[1] +
        m_fData[2]*rQ.m_fData[2] +
        m_fData[3]*rQ.m_fData[3];
}
//----------------------------------------------------------------------------
inline float SEQuaternionf::Normalize()
{
    float fLength = GetLength();

    if( fLength > SEMath<float>::ZERO_TOLERANCE )
    {
        float fInvLength = 1.0f / fLength;
        
        m_fData[0] *= fInvLength;
        m_fData[1] *= fInvLength;
        m_fData[2] *= fInvLength;
        m_fData[3] *= fInvLength;
    }
    else
    {
        fLength = 0.0f;

        m_fData[0] = 0.0f;
        m_fData[1] = 0.0f;
        m_fData[2] = 0.0f;
        m_fData[3] = 0.0f;
    }

    return fLength;
}
//----------------------------------------------------------------------------
inline void SEQuaternionf::GetInverse(SEQuaternionf& rDesQ) const
{
    float fNorm = m_fData[0]*m_fData[0] +
                  m_fData[1]*m_fData[1] +
                  m_fData[2]*m_fData[2] +
                  m_fData[3]*m_fData[3];

    if( fNorm > 0.0f )
    {
        float fInvNorm = 1.0f / fNorm;
        rDesQ.m_fData[0] = m_fData[0] * fInvNorm;
        rDesQ.m_fData[1] = -m_fData[1] * fInvNorm;
        rDesQ.m_fData[2] = -m_fData[2] * fInvNorm;
        rDesQ.m_fData[3] = -m_fData[3] * fInvNorm;
    }
    else
    {
        rDesQ.m_fData[0] = 0.0f;
        rDesQ.m_fData[1] = 0.0f;
        rDesQ.m_fData[2] = 0.0f;
        rDesQ.m_fData[3] = 0.0f;
    }
}
//----------------------------------------------------------------------------
inline void SEQuaternionf::GetConjugate(SEQuaternionf& rDesQ) const
{
    rDesQ.m_fData[0] = m_fData[0];
    rDesQ.m_fData[1] = -m_fData[1];
    rDesQ.m_fData[2] = -m_fData[2];
    rDesQ.m_fData[3] = -m_fData[3];
}
//----------------------------------------------------------------------------
inline int SEQuaternionf::CompareData(const SEQuaternionf& rQ) const
{
    return memcmp(&m_fData[0], &rQ.m_fData[0], 4*sizeof(float));
}
//----------------------------------------------------------------------------
inline SEQuaternionf& SEQuaternionf::FromAxisAngle(const SEVector3f& rAxis, 
    float fAngle)
{
    // 轴向量必须是单位向量,
    // 弧度制,
    // q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k).

    float fHalfAngle = 0.5f * fAngle;
    float fSin = SEMath<float>::Sin(fHalfAngle);
    m_fData[0] = SEMath<float>::Cos(fHalfAngle);
    m_fData[1] = fSin * rAxis[0];
    m_fData[2] = fSin * rAxis[1];
    m_fData[3] = fSin * rAxis[2];

    return *this;
}
//----------------------------------------------------------------------------
inline void SEQuaternionf::ToAxisAngle(SEVector3f& rAxis, float& rfAngle) 
    const
{
    float fSqrLength = m_fData[1]*m_fData[1] + m_fData[2]*m_fData[2] + 
        m_fData[3]*m_fData[3];

    if( fSqrLength > SEMath<float>::ZERO_TOLERANCE )
    {
        rfAngle = 2.0f * SEMath<float>::ACos(m_fData[0]);
        float fInvLength = SEMath<float>::InvSqrt(fSqrLength);
        
        rAxis[0] = m_fData[1] * fInvLength;
        rAxis[1] = m_fData[2] * fInvLength;
        rAxis[2] = m_fData[3] * fInvLength;
    }
    else
    {
        rfAngle = 0.0f;
        rAxis[0] = 1.0f;
        rAxis[1] = 0.0f;
        rAxis[2] = 0.0f;
    }
}
//----------------------------------------------------------------------------
inline SEQuaternionf& SEQuaternionf::FromRotationMatrix(const SEMatrix3f& 
    rRotMat)
{
    // Trace(R) = 2*Cos(theta) + 1.
    float fTrace = rRotMat(0, 0) + rRotMat(1, 1) + rRotMat(2, 2);
    float fRoot;

    if( fTrace > 0.0f )
    {
        fRoot = SEMath<float>::Sqrt(fTrace + 1.0f);
        m_fData[0] = 0.5f * fRoot; // Cos(theta/2)
        // fRoot = 1/(4*Cos(theta/2)).
        fRoot = 0.5f / fRoot;

        // 2*Sin(theta)*w0*fRoot = Sin(theta/2)*w0.
        m_fData[1] = (rRotMat(1, 2) - rRotMat(2, 1)) * fRoot;
        // 2*Sin(theta)*w1*fRoot = Sin(theta/2)*w1.
        m_fData[2] = (rRotMat(2, 0) - rRotMat(0, 2)) * fRoot;
        // 2*Sin(theta)*w2*fRoot = Sin(theta/2)*w2.
        m_fData[3] = (rRotMat(0, 1) - rRotMat(1, 0)) * fRoot;
    }
    else
    {
        int i = 0;
        if( rRotMat(1, 1) > rRotMat(0, 0) )
        {
            i = 1;
        }
        if( rRotMat(2, 2) > rRotMat(i, i) )
        {
            i = 2;
        }
        int j = m_iNext[i];
        int k = m_iNext[j];

        fRoot = SEMath<float>::Sqrt(rRotMat(i, i) - rRotMat(j, j) - 
            rRotMat(k, k) + 1.0f);
        float* pQData[3] = { &m_fData[1], &m_fData[2], &m_fData[3] };
        *pQData[i] = 0.5f * fRoot;
        fRoot = 0.5f / fRoot;

        m_fData[0] = (rRotMat(j, k) - rRotMat(k, j)) * fRoot;
        *pQData[j] = (rRotMat(i, j) + rRotMat(j, i)) * fRoot;
        *pQData[k] = (rRotMat(i, k) + rRotMat(k, i)) * fRoot;
    }

    return *this;
}
//----------------------------------------------------------------------------
inline void SEQuaternionf::ToRotationMatrix(SEMatrix3f& rRotMat) const
{
    float fTx  = 2.0f * m_fData[1];
    float fTy  = 2.0f * m_fData[2];
    float fTz  = 2.0f * m_fData[3];
    float fTwx = fTx * m_fData[0];
    float fTwy = fTy * m_fData[0];
    float fTwz = fTz * m_fData[0];
    float fTxx = fTx * m_fData[1];
    float fTxy = fTy * m_fData[1];
    float fTxz = fTz * m_fData[1];
    float fTyy = fTy * m_fData[2];
    float fTyz = fTz * m_fData[2];
    float fTzz = fTz * m_fData[3];

    rRotMat(0, 0) = 1.0f - (fTyy + fTzz);
    rRotMat(1, 0) = fTxy - fTwz;
    rRotMat(2, 0) = fTxz + fTwy;

    rRotMat(0, 1) = fTxy + fTwz;
    rRotMat(1, 1) = 1.0f - (fTxx + fTzz);
    rRotMat(2, 1) = fTyz - fTwx;

    rRotMat(0, 2) = fTxz - fTwy;
    rRotMat(1, 2) = fTyz + fTwx;
    rRotMat(2, 2) = 1.0f - (fTxx + fTyy);
}
//----------------------------------------------------------------------------
inline float SEQuaternionf::GetPitch() const
{
    return SEMath<float>::ATan2(2.0f*(Y*Z + W*X), W*W - X*X - Y*Y + Z*Z);
}
//----------------------------------------------------------------------------
inline float SEQuaternionf::GetYaw() const
{
    return SEMath<float>::ASin(-2.0f*(X*Z - W*Y));
}
//----------------------------------------------------------------------------
inline float SEQuaternionf::GetRoll() const
{
    return SEMath<float>::ATan2(2.0f*(X*Y + W*Z), W*W + X*X - Y*Y - Z*Z);
}
//----------------------------------------------------------------------------
inline SEVector3f SEQuaternionf::GetEulerAngles() const
{
    return SEVector3f(GetPitch(), GetYaw(), GetRoll());
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
inline double SEQuaterniond::GetLength() const
{
    return SEMath<double>::Sqrt(m_dData[0]*m_dData[0] +
                              m_dData[1]*m_dData[1] +
                              m_dData[2]*m_dData[2] +
                              m_dData[3]*m_dData[3]);
}
//----------------------------------------------------------------------------
inline double SEQuaterniond::GetSquaredLength() const
{
    return
        m_dData[0]*m_dData[0] +
        m_dData[1]*m_dData[1] +
        m_dData[2]*m_dData[2] +
        m_dData[3]*m_dData[3];
}
//----------------------------------------------------------------------------
inline double SEQuaterniond::Dot(const SEQuaterniond& rQ) const
{
    return 
        m_dData[0]*rQ.m_dData[0] +
        m_dData[1]*rQ.m_dData[1] +
        m_dData[2]*rQ.m_dData[2] +
        m_dData[3]*rQ.m_dData[3];
}
//----------------------------------------------------------------------------
inline double SEQuaterniond::Normalize()
{
    double dLength = GetLength();

    if( dLength > SEMath<double>::ZERO_TOLERANCE )
    {
        double dInvLength = 1.0 / dLength;
        
        m_dData[0] *= dInvLength;
        m_dData[1] *= dInvLength;
        m_dData[2] *= dInvLength;
        m_dData[3] *= dInvLength;
    }
    else
    {
        dLength = 0.0;

        m_dData[0] = 0.0;
        m_dData[1] = 0.0;
        m_dData[2] = 0.0;
        m_dData[3] = 0.0;
    }

    return dLength;
}
//----------------------------------------------------------------------------
inline void SEQuaterniond::GetInverse(SEQuaterniond& rDesQ) const
{
    double dNorm = m_dData[0]*m_dData[0] +
                   m_dData[1]*m_dData[1] +
                   m_dData[2]*m_dData[2] +
                   m_dData[3]*m_dData[3];

    if( dNorm > 0.0 )
    {
        double dInvNorm = 1.0 / dNorm;
        rDesQ.m_dData[0] = m_dData[0] * dInvNorm;
        rDesQ.m_dData[1] = -m_dData[1] * dInvNorm;
        rDesQ.m_dData[2] = -m_dData[2] * dInvNorm;
        rDesQ.m_dData[3] = -m_dData[3] * dInvNorm;
    }
    else
    {
        rDesQ.m_dData[0] = 0.0;
        rDesQ.m_dData[1] = 0.0;
        rDesQ.m_dData[2] = 0.0;
        rDesQ.m_dData[3] = 0.0;
    }
}
//----------------------------------------------------------------------------
inline void SEQuaterniond::GetConjugate(SEQuaterniond& rDesQ) const
{
    rDesQ.m_dData[0] = m_dData[0];
    rDesQ.m_dData[1] = -m_dData[1];
    rDesQ.m_dData[2] = -m_dData[2];
    rDesQ.m_dData[3] = -m_dData[3];
}
//----------------------------------------------------------------------------
inline int SEQuaterniond::CompareData(const SEQuaterniond& rQ) const
{
    return memcmp(&m_dData[0], &rQ.m_dData[0], 4*sizeof(double));
}
//----------------------------------------------------------------------------
inline SEQuaterniond& SEQuaterniond::FromAxisAngle(const SEVector3d& rAxis, 
    double dAngle)
{
    // 轴向量必须是单位向量,
    // 弧度制,
    // q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k).

    double dHalfAngle = 0.5 * dAngle;
    double dSin = SEMath<double>::Sin(dHalfAngle);
    m_dData[0] = SEMath<double>::Cos(dHalfAngle);
    m_dData[1] = dSin * rAxis[0];
    m_dData[2] = dSin * rAxis[1];
    m_dData[3] = dSin * rAxis[2];

    return *this;
}
//----------------------------------------------------------------------------
inline void SEQuaterniond::ToAxisAngle(SEVector3d& rAxis, double& rdAngle) 
    const
{
    double dSqrLength = m_dData[1]*m_dData[1] + m_dData[2]*m_dData[2] + 
        m_dData[3]*m_dData[3];

    if( dSqrLength > SEMath<double>::ZERO_TOLERANCE )
    {
        rdAngle = 2.0 * SEMath<double>::ACos(m_dData[0]);
        double dInvLength = SEMath<double>::InvSqrt(dSqrLength);
        
        rAxis[0] = m_dData[1] * dInvLength;
        rAxis[1] = m_dData[2] * dInvLength;
        rAxis[2] = m_dData[3] * dInvLength;
    }
    else
    {
        rdAngle = 0.0;
        rAxis[0] = 1.0;
        rAxis[1] = 0.0;
        rAxis[2] = 0.0;
    }
}
//----------------------------------------------------------------------------
inline SEQuaterniond& SEQuaterniond::FromRotationMatrix(const SEMatrix3d& 
    rRotMat)
{
    // Trace(R) = 2*Cos(theta) + 1.
    double dTrace = rRotMat(0, 0) + rRotMat(1, 1) + rRotMat(2, 2);
    double dRoot;

    if( dTrace > 0.0 )
    {
        dRoot = SEMath<double>::Sqrt(dTrace + 1.0);
        m_dData[0] = 0.5 * dRoot; // Cos(theta/2)
        // fRoot = 1/(4*Cos(theta/2)).
        dRoot = 0.5 / dRoot;

        // 2*Sin(theta)*w0*fRoot = Sin(theta/2)*w0.
        m_dData[1] = (rRotMat(1, 2) - rRotMat(2, 1)) * dRoot;
        // 2*Sin(theta)*w1*fRoot = Sin(theta/2)*w1.
        m_dData[2] = (rRotMat(2, 0) - rRotMat(0, 2)) * dRoot;
        // 2*Sin(theta)*w2*fRoot = Sin(theta/2)*w2.
        m_dData[3] = (rRotMat(0, 1) - rRotMat(1, 0)) * dRoot;
    }
    else
    {
        int i = 0;
        if( rRotMat(1, 1) > rRotMat(0, 0) )
        {
            i = 1;
        }
        if( rRotMat(2, 2) > rRotMat(i, i) )
        {
            i = 2;
        }
        int j = m_iNext[i];
        int k = m_iNext[j];

        dRoot = SEMath<double>::Sqrt(rRotMat(i, i) - rRotMat(j, j) - 
            rRotMat(k, k) + 1.0);
        double* pQData[3] = { &m_dData[1], &m_dData[2], &m_dData[3] };
        *pQData[i] = 0.5 * dRoot;
        dRoot = 0.5 / dRoot;

        m_dData[0] = (rRotMat(j, k) - rRotMat(k, j)) * dRoot;
        *pQData[j] = (rRotMat(i, j) + rRotMat(j, i)) * dRoot;
        *pQData[k] = (rRotMat(i, k) + rRotMat(k, i)) * dRoot;
    }

    return *this;
}
//----------------------------------------------------------------------------
inline void SEQuaterniond::ToRotationMatrix(SEMatrix3d& rRotMat) const
{
    double dTx  = 2.0 * m_dData[1];
    double dTy  = 2.0 * m_dData[2];
    double dTz  = 2.0 * m_dData[3];
    double dTwx = dTx * m_dData[0];
    double dTwy = dTy * m_dData[0];
    double dTwz = dTz * m_dData[0];
    double dTxx = dTx * m_dData[1];
    double dTxy = dTy * m_dData[1];
    double dTxz = dTz * m_dData[1];
    double dTyy = dTy * m_dData[2];
    double dTyz = dTz * m_dData[2];
    double dTzz = dTz * m_dData[3];

    rRotMat(0, 0) = 1.0 - (dTyy + dTzz);
    rRotMat(1, 0) = dTxy - dTwz;
    rRotMat(2, 0) = dTxz + dTwy;

    rRotMat(0, 1) = dTxy + dTwz;
    rRotMat(1, 1) = 1.0 - (dTxx + dTzz);
    rRotMat(2, 1) = dTyz - dTwx;

    rRotMat(0, 2) = dTxz - dTwy;
    rRotMat(1, 2) = dTyz + dTwx;
    rRotMat(2, 2) = 1.0 - (dTxx + dTyy);
}
//----------------------------------------------------------------------------