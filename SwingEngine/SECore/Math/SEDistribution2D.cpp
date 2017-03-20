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
#include "SEDistribution2D.h"
#include "SEMath.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDistribution2D::SEDistribution2D(const float* pFunc, int iUCount, 
    int iVCount)
{
    SE_ASSERT( pFunc && iUCount > 0 && iVCount > 0 );

    m_ConditionalV.reserve(iVCount);
    for( int v = 0; v < iVCount; ++v )
    {
        // Compute conditional sampling distribution for v.
        m_ConditionalV.push_back(
            SE_NEW SEDistribution1D(&pFunc[v * iUCount], iUCount));
    }

    // Compute marginal sampling distribution.
    std::vector<float> marginalFunc;
    marginalFunc.reserve(iVCount);
    for( int v = 0; v < iVCount; ++v )
    {
        marginalFunc.push_back(m_ConditionalV[v]->m_fFuncInt);
    }
    m_pMarginal = SE_NEW SEDistribution1D(&marginalFunc[0], iVCount);
}
//----------------------------------------------------------------------------
SEDistribution2D::~SEDistribution2D()
{
    SE_DELETE m_pMarginal;
    for( int i = 0; i < (int)m_ConditionalV.size(); ++i )
    {
        SE_DELETE m_ConditionalV[i];
    }
}
//----------------------------------------------------------------------------
void SEDistribution2D::SampleContinuous(float fU0, float fU1, float fUV[2],
    float* pPDF) const
{
    float pdfs[2];
    int v;
    fUV[1] = m_pMarginal->SampleContinuous(fU1, &pdfs[1], &v);
    fUV[0] = m_ConditionalV[v]->SampleContinuous(fU0, &pdfs[0]);
    *pPDF = pdfs[0] * pdfs[1];
}
//----------------------------------------------------------------------------
float SEDistribution2D::Pdf(float fU, float fV) const
{
    int iU = SE_Clamp((int)(fU * m_ConditionalV[0]->m_iCount), 0, 
        m_ConditionalV[0]->m_iCount - 1);
    int iV = SE_Clamp((int)(fV * m_pMarginal->m_iCount), 0, 
        m_pMarginal->m_iCount - 1);

    if( m_ConditionalV[iV]->m_fFuncInt * m_pMarginal->m_fFuncInt == 0.0f )
    {
        SE_ASSERT( false );

        return 0.0f;
    }

    return (m_ConditionalV[iV]->m_aFunc[iU] * m_pMarginal->m_aFunc[iV]) /
           (m_ConditionalV[iV]->m_fFuncInt * m_pMarginal->m_fFuncInt);
}
//----------------------------------------------------------------------------