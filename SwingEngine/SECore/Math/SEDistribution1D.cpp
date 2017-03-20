// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017
//
// This part of Swing Engine is based on PBRT.
/*
    pbrt source code Copyright(c) 1998-2012 Matt Pharr and Greg Humphreys.

    This file is part of pbrt.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

#include "SECorePCH.h"
#include "SEDistribution1D.h"
#include "SEMath.h"
#include <algorithm>

using namespace Swing;

//----------------------------------------------------------------------------
SEDistribution1D::SEDistribution1D(const float* pFunc, int iCount)
{
    SE_ASSERT( pFunc && iCount > 0 );

    m_iCount = iCount;
    m_aFunc = SE_NEW float[iCount];
    memcpy(m_aFunc, pFunc, iCount*sizeof(float));
    m_aCDF = SE_NEW float[iCount + 1];

    // Compute integral of step function at x_i.
    m_aCDF[0] = 0.0f;
    float fInvCount = 1.0f / (float)iCount;
    for( int i = 1; i < m_iCount + 1; ++i )
    {
        m_aCDF[i] = m_aCDF[i - 1] + m_aFunc[i - 1] * fInvCount;
    }

    // Transform step function integral into CDF.
    m_fFuncInt = m_aCDF[m_iCount];
    if( m_fFuncInt == 0.0f )
    {
        SE_ASSERT( false );
        for( int i = 1; i < iCount + 1; ++i )
        {
            m_aCDF[i] = float(i) * fInvCount;
        }
    }
    else
    {
        float fInvFuncInt = 1.0f / m_fFuncInt;
        for( int i = 1; i < iCount + 1; ++i )
        {
            m_aCDF[i] *= fInvFuncInt;
        }
    }
}
//----------------------------------------------------------------------------
SEDistribution1D::~SEDistribution1D()
{
    SE_DELETE[] m_aFunc;
    SE_DELETE[] m_aCDF;
}
//----------------------------------------------------------------------------
float SEDistribution1D::SampleContinuous(float fU, float* pPDF, 
    int* pOffset) const
{
    // Find surrounding CDF segments and offset.
    float* ptr = std::upper_bound(m_aCDF, m_aCDF + m_iCount + 1, fU);
    int tempValue = int(ptr - m_aCDF - 1);
    int iOffset = SE_MAX(0, tempValue);
    if( pOffset )
    {
        *pOffset = iOffset;
    }
    SE_ASSERT( iOffset < m_iCount );
    SE_ASSERT( fU >= m_aCDF[iOffset] && fU < m_aCDF[iOffset + 1] );

    // Compute offset along CDF segment.
    float du = 
        (fU - m_aCDF[iOffset]) / (m_aCDF[iOffset + 1] - m_aCDF[iOffset]);
    SE_ASSERT( !isnan(du) );

    // Compute PDF for sampled offset.
    if( pPDF )
    {
        *pPDF = m_aFunc[iOffset] / m_fFuncInt;
    }

    return (iOffset + du) / m_iCount;
}
//----------------------------------------------------------------------------
int SEDistribution1D::SampleDiscrete(float fU, float *pPDF) const
{
    // Find surrounding CDF segments and offset.
    float* ptr = std::upper_bound(m_aCDF, m_aCDF + m_iCount + 1, fU);
    int tempValue = int(ptr - m_aCDF - 1);
    int iOffset = SE_MAX(0, tempValue);
    SE_ASSERT( iOffset < m_iCount );
    SE_ASSERT( fU >= m_aCDF[iOffset] && fU < m_aCDF[iOffset + 1] );

    if( pPDF )
    {
        *pPDF = m_aFunc[iOffset] / (m_fFuncInt * m_iCount);
    }

    return iOffset;
}
//----------------------------------------------------------------------------