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

//----------------------------------------------------------------------------
inline float SE_CosTheta(const SEVector3f& w)
{
    return w.Z;
}
//----------------------------------------------------------------------------
inline float SE_AbsCosTheta(const SEVector3f& w)
{
    return fabsf(w.Z);
}
//----------------------------------------------------------------------------
inline float SE_SinTheta2(const SEVector3f& w)
{
    float sinTheta2 = 1.0f - SE_CosTheta(w)*SE_CosTheta(w);

    return SE_MAX(0.0f, sinTheta2);
}
//----------------------------------------------------------------------------
inline float SE_SinTheta(const SEVector3f& w)
{
    return sqrtf(SE_SinTheta2(w));
}
//----------------------------------------------------------------------------
inline float SE_CosPhi(const SEVector3f& w)
{
    float sinTheta = SE_SinTheta(w);
    if( sinTheta == 0.0f )
    {
        return 1.0f;
    }

    return SE_Clamp(w.X / sinTheta, -1.0f, 1.0f);
}
//----------------------------------------------------------------------------
inline float SE_SinPhi(const SEVector3f& w)
{
    float sinTheta = SE_SinTheta(w);
    if( sinTheta == 0.0f )
    {
        return 0.0f;
    }

    return SE_Clamp(w.Y / sinTheta, -1.0f, 1.0f);
}
//----------------------------------------------------------------------------
inline bool SE_SameHemisphere(const SEVector3f& w, const SEVector3f& wp)
{
    return w.Z * wp.Z > 0.0f;
}
//----------------------------------------------------------------------------