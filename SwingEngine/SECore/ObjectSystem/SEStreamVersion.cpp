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

#include "SECorePCH.h"
#include "SEStreamVersion.h"

using namespace Swing;

const int SEStreamVersion::MAJOR = 2;
const int SEStreamVersion::MINOR = 0;
const char SEStreamVersion::LABEL[] = "Swing Engine Object File 2.00";
const int SEStreamVersion::LENGTH = 30;
const SEStreamVersion SEStreamVersion::CURRENT(MAJOR, MINOR);

//----------------------------------------------------------------------------
SEStreamVersion::SEStreamVersion(int iMajor, int iMinor)
{
    m_iMajor = iMajor;
    m_iMinor = iMinor;
}
//----------------------------------------------------------------------------
SEStreamVersion::SEStreamVersion(const char* pString)
{
    m_iMajor = -1;
    m_iMinor = -1;

    if( pString 
        &&  strlen(pString) >= LENGTH-1 
        &&  pString[LENGTH-1] == 0 
        &&  strncmp(pString, LABEL, LENGTH-5) == 0 )  // 5 = strlen(" 2.xx")
    {
        // The version string is "x.yy".
        const char* acVersion = pString + LENGTH - 5;
        m_iMajor = (int)(acVersion[0]-'0');
        m_iMinor = 10*(int)(acVersion[2]-'0') + (int)(acVersion[3]-'0');
    }
}
//----------------------------------------------------------------------------
bool SEStreamVersion::IsValid() const
{
    return m_iMajor == SEStreamVersion::MAJOR
        && 0 <= m_iMinor && m_iMinor < 100;
}
//----------------------------------------------------------------------------
int SEStreamVersion::GetMajor() const
{
    return m_iMajor;
}
//----------------------------------------------------------------------------
int SEStreamVersion::GetMinor() const
{
    return m_iMinor;
}
//----------------------------------------------------------------------------
int SEStreamVersion::GetCombined() const
{
    return 100*m_iMajor + m_iMinor;
}
//----------------------------------------------------------------------------
bool SEStreamVersion::operator==(const SEStreamVersion& rVersion) const
{
    SE_ASSERT( IsValid() && rVersion.IsValid() );
    return GetCombined() == rVersion.GetCombined();
}
//----------------------------------------------------------------------------
bool SEStreamVersion::operator!=(const SEStreamVersion& rVersion) const
{
    SE_ASSERT( IsValid() && rVersion.IsValid() );
    return GetCombined() != rVersion.GetCombined();
}
//----------------------------------------------------------------------------
bool SEStreamVersion::operator<(const SEStreamVersion& rVersion) const
{
    SE_ASSERT( IsValid() && rVersion.IsValid() );
    return GetCombined() < rVersion.GetCombined();
}
//----------------------------------------------------------------------------
bool SEStreamVersion::operator<=(const SEStreamVersion& rVersion) const
{
    SE_ASSERT( IsValid() && rVersion.IsValid() );
    return GetCombined() <= rVersion.GetCombined();
}
//----------------------------------------------------------------------------
bool SEStreamVersion::operator>(const SEStreamVersion& rVersion) const
{
    SE_ASSERT( IsValid() && rVersion.IsValid() );
    return GetCombined() > rVersion.GetCombined();
}
//----------------------------------------------------------------------------
bool SEStreamVersion::operator>=(const SEStreamVersion& rVersion) const
{
    SE_ASSERT( IsValid() && rVersion.IsValid() );
    return GetCombined() >= rVersion.GetCombined();
}
//----------------------------------------------------------------------------
