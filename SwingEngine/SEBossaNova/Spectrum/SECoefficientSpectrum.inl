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

//----------------------------------------------------------------------------
template <int nSamples>
SECoefficientSpectrum<nSamples>::SECoefficientSpectrum(float v)
{
    for( int i = 0; i < nSamples; ++i )
    {
        c[i] = v;
    }
    SE_ASSERT( !HasNaNs() );
}
//----------------------------------------------------------------------------
#ifdef _DEBUG
template <int nSamples>
SECoefficientSpectrum<nSamples>::SECoefficientSpectrum(
    const SECoefficientSpectrum& s)
{
    SE_ASSERT( !s.HasNaNs() );
    for( int i = 0; i < nSamples; ++i )
    {
        c[i] = s.c[i];
    }
}
//----------------------------------------------------------------------------
template <int nSamples>
SECoefficientSpectrum<nSamples>& SECoefficientSpectrum<nSamples>::operator=(
    const SECoefficientSpectrum& s)
{
    SE_ASSERT( !s.HasNaNs() );
    for( int i = 0; i < nSamples; ++i )
    {
        c[i] = s.c[i];
    }
    return *this;
}
#endif
//----------------------------------------------------------------------------
template <int nSamples>
SECoefficientSpectrum<nSamples>& SECoefficientSpectrum<nSamples>::operator+=(
    const SECoefficientSpectrum& s2) 
{
    SE_ASSERT( !s2.HasNaNs() );
    for( int i = 0; i < nSamples; ++i )
    {
        c[i] += s2.c[i];
    }
    return *this;
}
//----------------------------------------------------------------------------
template <int nSamples>
SECoefficientSpectrum<nSamples> SECoefficientSpectrum<nSamples>::operator+(
    const SECoefficientSpectrum& s2) const 
{
    SE_ASSERT( !s2.HasNaNs() );
    SECoefficientSpectrum<nSamples> ret = *this;

    for( int i = 0; i < nSamples; ++i )
    {
        ret.c[i] += s2.c[i];
    }
    return ret;
}
//----------------------------------------------------------------------------
template <int nSamples>
SECoefficientSpectrum<nSamples> SECoefficientSpectrum<nSamples>::operator-(
    const SECoefficientSpectrum& s2) const
{
    SE_ASSERT( !s2.HasNaNs() );
    SECoefficientSpectrum<nSamples> ret = *this;

    for( int i = 0; i < nSamples; ++i )
    {
        ret.c[i] -= s2.c[i];
    }
    return ret;
}
//----------------------------------------------------------------------------
template <int nSamples>
SECoefficientSpectrum<nSamples> SECoefficientSpectrum<nSamples>::operator/(
    const SECoefficientSpectrum& s2) const 
{
    SE_ASSERT( !s2.HasNaNs() );
    SECoefficientSpectrum<nSamples> ret = *this;

    for( int i = 0; i < nSamples; ++i )
    {
        ret.c[i] /= s2.c[i];
    }
    return ret;
}
//----------------------------------------------------------------------------
template <int nSamples>
SECoefficientSpectrum<nSamples> SECoefficientSpectrum<nSamples>::operator*(
    const SECoefficientSpectrum& sp) const 
{
    SE_ASSERT( !sp.HasNaNs() );
    SECoefficientSpectrum<nSamples> ret = *this;

    for( int i = 0; i < nSamples; ++i )
    {
        ret.c[i] *= sp.c[i];
    }
    return ret;
}
//----------------------------------------------------------------------------
template <int nSamples>
SECoefficientSpectrum<nSamples>& SECoefficientSpectrum<nSamples>::operator*=(
    const SECoefficientSpectrum &sp) 
{
    SE_ASSERT( !sp.HasNaNs() );
    for( int i = 0; i < nSamples; ++i )
    {
        c[i] *= sp.c[i];
    }
    return *this;
}
//----------------------------------------------------------------------------
template <int nSamples>
SECoefficientSpectrum<nSamples> SECoefficientSpectrum<nSamples>::operator*(
    float a) const 
{
    SECoefficientSpectrum<nSamples> ret = *this;
    for( int i = 0; i < nSamples; ++i )
    {
        ret.c[i] *= a;
    }
    SE_ASSERT( !ret.HasNaNs() );
    return ret;
}
//----------------------------------------------------------------------------
template <int nSamples>
SECoefficientSpectrum<nSamples>& SECoefficientSpectrum<nSamples>::operator*=(
    float a) 
{
    for( int i = 0; i < nSamples; ++i )
    {
        c[i] *= a;
    }
    SE_ASSERT( !HasNaNs() );
    return *this;
}
//----------------------------------------------------------------------------
template <int nSamples>
SECoefficientSpectrum<nSamples> SECoefficientSpectrum<nSamples>::operator/(
    float a) const 
{
    SE_ASSERT( !isnan(a) );
    SECoefficientSpectrum<nSamples> ret = *this;

    float inva = 1.0f / a;
    for( int i = 0; i < nSamples; ++i )
    {
        ret.c[i] *= inva;
    }
    SE_ASSERT(!ret.HasNaNs());
    return ret;
}
//----------------------------------------------------------------------------
template <int nSamples>
SECoefficientSpectrum<nSamples>& SECoefficientSpectrum<nSamples>::operator/=(
    float a) 
{
    SE_ASSERT( !isnan(a) );

    float inva = 1.0f / a;
    for( int i = 0; i < nSamples; ++i )
    {
        c[i] *= inva;
    }
    return *this;
}
//----------------------------------------------------------------------------
template <int nSamples>
inline float SECoefficientSpectrum<nSamples>::operator[](int i) const
{
    return c[i];
}
//----------------------------------------------------------------------------
template <int nSamples>
inline float& SECoefficientSpectrum<nSamples>::operator[](int i)
{
    return c[i];
}
//----------------------------------------------------------------------------
template <int nSamples>
bool SECoefficientSpectrum<nSamples>::operator==(
    const SECoefficientSpectrum<nSamples>& sp) const
{
    for( int i = 0; i < nSamples; ++i )
    {
        if( c[i] != sp.c[i] ) 
        {
            return false;
        }
    }
    return true;
}
//----------------------------------------------------------------------------
template <int nSamples>
bool SECoefficientSpectrum<nSamples>::operator!=(
    const SECoefficientSpectrum<nSamples>& sp) const 
{
    return !(*this == sp);
}
//----------------------------------------------------------------------------
template <int nSamples>
bool SECoefficientSpectrum<nSamples>::IsBlack() const 
{
    for( int i = 0; i < nSamples; ++i )
    {
        if( c[i] != 0.0f )
        {
            return false;
        }
    }
    return true;
}
//----------------------------------------------------------------------------
template <int nSamples>
SECoefficientSpectrum<nSamples> SECoefficientSpectrum<nSamples>::operator-(
    ) const 
{
    SECoefficientSpectrum<nSamples> ret;
    for( int i = 0; i < nSamples; ++i )
    {
        ret.c[i] = -c[i];
    }
    return ret;
}
//----------------------------------------------------------------------------
template <int nSamples>
SECoefficientSpectrum<nSamples> SECoefficientSpectrum<nSamples>::Clamp(
    float low, float high) const 
{
    SECoefficientSpectrum<nSamples> ret;
    for( int i = 0; i < nSamples; ++i )
    {
        ret.c[i] = SE_Clamp(c[i], low, high);
    }
    SE_ASSERT( !ret.HasNaNs() );
    return ret;
}
//----------------------------------------------------------------------------
template <int nSamples>
bool SECoefficientSpectrum<nSamples>::HasNaNs() const
{
    for( int i = 0; i < nSamples; ++i )
    {
        if( isnan(c[i]) )
        {
            return true;
        }
    }
    return false;
}
//----------------------------------------------------------------------------
//template <int nSamples>
//bool SECoefficientSpectrum<nSamples>::Write(FILE* f) const 
//{
//    for( int i = 0; i < nSamples; ++i )
//    {
//        if( fprintf(f, "%f ", c[i]) < 0 )
//        {
//            return false;
//        }
//    }
//    return true;
//}
//----------------------------------------------------------------------------
//template <int nSamples>
//bool SECoefficientSpectrum<nSamples>::Read(FILE* f)
//{
//    for( int i = 0; i < nSamples; ++i )
//    {
//        if( fscanf(f, "%f ", &c[i]) != 1 )
//        {
//            return false;
//        }
//    }
//    return true;
//}
//----------------------------------------------------------------------------
//template <int nSamples>
//void SECoefficientSpectrum<nSamples>::Print(FILE* f) const 
//{
//    fprintf(f, "[ ");
//    for( int i = 0; i < nSamples; ++i )
//    {
//        fprintf(f, "%f", c[i]);
//        if( i != nSamples - 1 )
//        {
//            fprintf(f, ", ");
//        }
//    }
//    fprintf(f, "]");
//}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
template <int nSamples>
inline SECoefficientSpectrum<nSamples> operator*(float a, 
    const SECoefficientSpectrum<nSamples>& s) 
{
    SE_ASSERT( !isnan(a) && !s.HasNaNs() );

    return s * a;
}
//----------------------------------------------------------------------------
template <int nSamples>
SECoefficientSpectrum<nSamples> Sqrt(const SECoefficientSpectrum<nSamples>& s)
{
    SECoefficientSpectrum<nSamples> ret;
    for( int i = 0; i < nSamples; ++i )
    {
        ret[i] = SEMathf::Sqrt(s[i]);
    }
    SE_ASSERT( !ret.HasNaNs() );

    return ret;
}
//----------------------------------------------------------------------------
template <int nSamples> 
inline SECoefficientSpectrum<nSamples> Pow(
    const SECoefficientSpectrum<nSamples>& s, float e)
{
    SECoefficientSpectrum<nSamples> ret;
    for( int i = 0; i < nSamples; ++i )
    {
        ret[i] = SEMathf::Pow(s[i], e);
    }
    SE_ASSERT( !ret.HasNaNs() );

    return ret;
}
//----------------------------------------------------------------------------
template <int nSamples>
SECoefficientSpectrum<nSamples> Exp(const SECoefficientSpectrum<nSamples>& s) 
{
    SECoefficientSpectrum<nSamples> ret;
    for( int i = 0; i < nSamples; ++i )
    {
        ret[i] = SEMathf::Exp(s[i]);
    }
    SE_ASSERT( !ret.HasNaNs() );

    return ret;
}
//----------------------------------------------------------------------------