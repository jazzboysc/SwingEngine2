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

#include "SEBossaNovaPCH.h"
#include "SEFresnelDielectric.h"
#include "SEBSDFUtility.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEFresnelDielectric::SEFresnelDielectric(float ei, float et)
    :
    Eta_i(ei), 
    Eta_t(et)
{
}
//----------------------------------------------------------------------------
SESpectrum SEFresnelDielectric::Evaluate(float cosi) const
{
    cosi = SE_Clamp(cosi, -1.0f, 1.0f);

    // Compute indices of refraction for dielectric.
    bool entering = cosi > 0.0f;
    float ei = Eta_i, et = Eta_t;
    if( !entering )
    {
        // Swap data.
        float tmp = ei;
        ei = et;
        et = tmp;
    }

    // Compute sint using Snell's law.
    float tempValue = 1.0f - cosi*cosi;
    float sint = ei/et * sqrtf(SE_MAX(0.0f, tempValue));
    if( sint >= 1.0f )
    {
        // Handle total internal reflection.
        return 1.0f;
    }
    else
    {
        tempValue = 1.0f - sint*sint;
        float cost = sqrtf(SE_MAX(0.0f, tempValue));

        return SE_FresnelDielectric(fabsf(cosi), cost, ei, et);
    }
}
//----------------------------------------------------------------------------