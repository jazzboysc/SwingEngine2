// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015
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
#include "SEBSDFUtility.h"

using namespace Swing;

//----------------------------------------------------------------------------
// Fresnel functions.
//----------------------------------------------------------------------------
SESpectrum Swing::SE_FresnelDielectric(float cosi, float cost, 
    const SESpectrum& etai, const SESpectrum& etat)
{
    SESpectrum Rparl = ((etat * cosi) - (etai * cost)) /
                       ((etat * cosi) + (etai * cost));
    SESpectrum Rperp = ((etai * cosi) - (etat * cost)) /
                       ((etai * cosi) + (etat * cost));

    return (Rparl*Rparl + Rperp*Rperp) * 0.5f;
}
//----------------------------------------------------------------------------
SESpectrum Swing::SE_FresnelConductor(float cosi, const SESpectrum& eta, 
    const SESpectrum& k)
{
    SESpectrum tmp = (eta*eta + k*k) * cosi*cosi;
    SESpectrum Rparl2 = (tmp - (2.0f * eta * cosi) + 1) /
                        (tmp + (2.0f * eta * cosi) + 1);

    SESpectrum tmp_f = eta*eta + k*k;
    SESpectrum Rperp2 =
        (tmp_f - (2.0f * eta * cosi) + cosi*cosi) /
        (tmp_f + (2.0f * eta * cosi) + cosi*cosi);

    return (Rparl2 + Rperp2) * 0.5f;
}
//----------------------------------------------------------------------------