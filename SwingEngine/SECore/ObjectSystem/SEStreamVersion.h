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

#ifndef Swing_StreamVersion_H
#define Swing_StreamVersion_H

#include "SECoreLIB.h"
#include "SEPlatforms.h"
#include "SESystem.h"

// Scene graphs are stored in files with extension seof. The header is of the
// form "Swing Engine Object File 2.xx" where the major version is 2 and the
// the minor version is xx in [00,99]. The length of the string is 29, but
// the null terminator is written to disk, so total number of file bytes used
// by the version is 30. The current version is "2.00"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20080329
//----------------------------------------------------------------------------
class SE_CORE_API SEStreamVersion
{
public:
    static const int MAJOR;     // 2
    static const int MINOR;     // 0
    static const char LABEL[];  // "Swing Engine Object File 2.00"
    static const int LENGTH;    // 30 = strlen(LABEL)+1
    static const SEStreamVersion CURRENT;

    SEStreamVersion(int iMajor = MAJOR, int iMinor = MINOR);
    SEStreamVersion(const char* pString);

    int GetMajor(void) const;
    int GetMinor(void) const;

    // The version is valid if major is 2 and minor in [0,99].
    bool IsValid(void) const;

    // For comparisons of versions. This is useful in the SEStream support in
    // an SEObject-derived class whenever a change to that class causes a file
    // format change.
    bool operator == (const SEStreamVersion& rVersion) const;
    bool operator != (const SEStreamVersion& rVersion) const;
    bool operator <  (const SEStreamVersion& rVersion) const;
    bool operator <= (const SEStreamVersion& rVersion) const;
    bool operator >  (const SEStreamVersion& rVersion) const;
    bool operator >= (const SEStreamVersion& rVersion) const;

protected:
    int GetCombined(void) const;  // 100*major + minor

    int m_iMajor, m_iMinor;
};

}

#endif
