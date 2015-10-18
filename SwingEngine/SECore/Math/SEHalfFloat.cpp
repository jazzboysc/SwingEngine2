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
#include "SEHalfFloat.h"

namespace Swing
{

//----------------------------------------------------------------------------
SEHalfFloat ToHalf(float fValue)
{
    unsigned int uiBits = *(unsigned int*)&fValue;
    unsigned short usBiasExp = (unsigned short)((uiBits & 0x7F800000) >> 23);
    if( usBiasExp >= 0x0071 )
    {
        if( usBiasExp <= 0x008E )
        {
            if( usBiasExp != 0 )
            {
                // Truncate 23-bit mantissa to 10 bits.
                unsigned short usSignBit =
                    (unsigned short)((uiBits & 0x80000000) >> 16);
                unsigned int uiMantissa = (uiBits & 0x007FFFFF) >> 13;
                usBiasExp = (usBiasExp - 0x0070) << 10;
                return (SEHalfFloat)(usSignBit | usBiasExp | uiMantissa);
            }
            else
            {
                // E = 0 (alternate encoding of zero, M does not matter)
                return (unsigned short)0;
            }
        }
        else
        {
            // E = 30, M = 1023 (largest magnitude half-float)
            unsigned short usSignBit =
                (unsigned short)((uiBits & 0x80000000) >> 16);
            return usSignBit | (unsigned short)0x7BFF;
        }
    }
    else
    {
        // E = 1, M = 0 (smallest magnitude half-float)
        unsigned short usSignBit = 
            (unsigned short)((uiBits & 0x80000000) >> 16);
        return usSignBit | (unsigned short)0x0400;
    }
}
//----------------------------------------------------------------------------
float ToFloat(SEHalfFloat hfValue)
{
    unsigned int uiBiasExp  = (unsigned int)(hfValue & 0x7C00) >> 10;
    if( uiBiasExp != 0 )
    {
        unsigned int uiSignBit  = (unsigned int)(hfValue & 0x8000) << 16;
        unsigned int uiMantissa = (unsigned int)(hfValue & 0x03FF) << 13;
        uiBiasExp = (uiBiasExp + 0x0070) << 23;
        unsigned int uiResult = uiSignBit | uiBiasExp | uiMantissa;
        return *(float*)&uiResult;
    }
    else
    {
        // E = 0 (alternate encoding of zero, M does not matter)
        return 0.0f;
    }
}
//----------------------------------------------------------------------------

}
