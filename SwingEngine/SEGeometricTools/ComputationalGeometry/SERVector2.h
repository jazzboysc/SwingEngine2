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

#ifndef Swing_RVector2_H
#define Swing_RVector2_H

#include "SEGeometricToolsLIB.h"
#include "SETRVector.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081202
//----------------------------------------------------------------------------
template <int ISIZE>
class SERVector2 : public SETRVector<2,  ISIZE>
{
public:
    // construction
    SERVector2();
    SERVector2(const SERVector2& rV);

#ifdef SE_USING_VC70
    SERVector2(const SETRVector<2,  ISIZE>& rV)
    {
        // The inline body is here because of an apparent MSVC++ .NET 2002
        // compiler bug. If placed in the *.inl file, the compiler complains:
        //
        //   error C2244: 'Swing::SERVector2<>::__ctor' : unable to match 
        //       function definition to an existing declaration
        //   definition
        //       'Swing::SERVector2<>::SERVector2(const Swing::SETRVector<2, 
        //       > &)' existing declarations
        //       'Swing::SERVector2<>::SERVector2(const Swing::SETRational<
        //       > &, const Swing::SETRational<> &)'
        //       'Swing::SERVector2<>::SERVector2(const Swing::SETRVector<2, 
        //       > &)'
        //       'Swing::SERVector2<>::SERVector2(const Swing::SERVector2<
        //       > &)' 'Swing::SERVector2<>::SERVector2(void)'
        // The "definition" is in the "existing declarations" list, so I do
        // not know what the compiler is complaining about.

        m_aTuple[0] = rV[0];
        m_aTuple[1] = rV[1];
    }
#else
    SERVector2(const SETRVector<2,  ISIZE>& rV);
#endif

    SERVector2(const SETRational<ISIZE>& rX,  const SETRational<ISIZE>& rY);

    // member access
    SETRational<ISIZE> X(void) const;
    SETRational<ISIZE>& X(void);
    SETRational<ISIZE> Y(void) const;
    SETRational<ISIZE>& Y(void);

    // assignment
    SERVector2& operator = (const SERVector2& rV);

#ifdef SE_USING_VC70
    SERVector2& operator = (const SETRVector<2,  ISIZE>& rV)
    {
        // The inline body is here because of an apparent MSVC++ .NET 2002
        // compiler bug.  If placed in the *.inl file, the compiler complains:
        //
        //   error C2244: 'Swing::SERVector2<>::operator`='' : unable to match
        //       function definition to an existing declaration
        //   definition
        //       'Swing::SERVector2<> &Swing::SERVector2<>::operator =(
        //            const Swing::SETRVector<2, > &)'
        //   existing declarations
        //       'Swing::SERVector2<> &Swing::SERVector2<>::operator =(
        //            const Swing::SETRVector<2, > &)'
        //       'Swing::SERVector2<> &Swing::SERVector2<>::operator =(
        //            const Swing::SERVector2<> &)'

        m_aTuple[0] = rV[0];
        m_aTuple[1] = rV[1];

        return *this;
    }
#else
    SERVector2& operator = (const SETRVector<2,  ISIZE>& rV);
#endif

    // returns Dot(this, V)
    SETRational<ISIZE> Dot(const SERVector2& rV) const;

    // returns (y, -x)
    SERVector2 Perp(void) const;

    // returns Cross((x, y, 0), (V.x, V.y, 0)) = x*V.y - y*V.x
    SETRational<ISIZE> DotPerp(const SERVector2& rV) const;

protected:
    using SETRVector<2,  ISIZE>::m_aTuple;
};

#include "SERVector2.inl"

}

#endif
