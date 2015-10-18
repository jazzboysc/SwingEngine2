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
inline bool SETransformation::IsIdentity() const
{
    return m_bIsIdentity;
}
//----------------------------------------------------------------------------
inline bool SETransformation::IsSRMatrix() const
{
    return m_bIsSRMatrix;
}
//----------------------------------------------------------------------------
inline bool SETransformation::IsUniformScale() const
{
    return m_bIsSRMatrix && m_bIsUniformScale;
}
//----------------------------------------------------------------------------
inline const SEMatrix3f& SETransformation::GetRotate() const
{
    SE_ASSERT( m_bIsSRMatrix );

    return m_Matrix;
}
//----------------------------------------------------------------------------
inline const SEMatrix3f& SETransformation::GetMatrix() const
{
    return m_Matrix;
}
//----------------------------------------------------------------------------
inline const SEVector3f& SETransformation::GetTranslate() const
{
    return m_Translate;
}
//----------------------------------------------------------------------------
inline const SEVector3f& SETransformation::GetScale() const
{
    SE_ASSERT( m_bIsSRMatrix );

    return m_Scale;
}
//----------------------------------------------------------------------------
inline float SETransformation::GetUniformScale() const
{
    SE_ASSERT( m_bIsSRMatrix && m_bIsUniformScale );

	return m_Scale.X;
}
//----------------------------------------------------------------------------



