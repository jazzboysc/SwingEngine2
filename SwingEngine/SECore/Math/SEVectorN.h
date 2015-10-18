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

#ifndef Swing_VectorN_H
#define Swing_VectorN_H

#include "SECoreLIB.h"
#include "SEMath.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Ê±¼ä:20070706
//----------------------------------------------------------------------------
class SE_CORE_API SEVectorNf
{
public:
    SEVectorNf(int iSize = 0);
    SEVectorNf(int iSize, const float* pData);
    SEVectorNf(const SEVectorNf& rVec);
    ~SEVectorNf(void);

public:
    inline void SetSize(int iSize);
    inline int GetSize(void) const;

    operator float* (void);
    operator const float* (void) const;
    float operator [] (int i) const;
    float& operator [] (int i);

    SEVectorNf& operator = (const SEVectorNf& rVec);

    bool operator == (const SEVectorNf& rVec) const;
    bool operator != (const SEVectorNf& rVec) const;
    bool operator <  (const SEVectorNf& rVec) const;
    bool operator <= (const SEVectorNf& rVec) const;
    bool operator >  (const SEVectorNf& rVec) const;
    bool operator >= (const SEVectorNf& rVec) const;

    SEVectorNf operator + (const SEVectorNf& rRhsVec) const;
    SEVectorNf operator - (const SEVectorNf& rRhsVec) const;
    SEVectorNf operator * (float fScalar) const;
    SEVectorNf operator / (float fScalar) const;
    SEVectorNf operator - (void) const;

    SEVectorNf& operator += (const SEVectorNf& rRhsVec);
    SEVectorNf& operator -= (const SEVectorNf& rRhsVec);
    SEVectorNf& operator *= (float fScalar);
    SEVectorNf& operator /= (float fScalar);

    SE_CORE_API friend SEVectorNf operator * (float fLhsScalar,
        const SEVectorNf& rRhsVec);

    inline float GetLength(void) const;
    inline float GetSquaredLength(void) const;
    inline float Dot(const SEVectorNf& rRhsVec) const;
    inline float Normalize(void);

private:
    inline int CompareData(const SEVectorNf& rVec) const;

    int m_iSize;
    float* m_pData;
};

#include "SEVectorN.inl"

}

#endif