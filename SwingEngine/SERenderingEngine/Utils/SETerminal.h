//----------------------------------------------------------------------------
// Graphics framework for real-time GI study.
// Che Sun at Worcester Polytechnic Institute, Fall 2013.
//----------------------------------------------------------------------------

#ifndef Swing_Terminal_H
#define Swing_Terminal_H

#include "SERenderingEngineLIB.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 12/12/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SETerminal
{
public:
    SETerminal();
    ~SETerminal();

    enum OutputColor
    {
        OC_Success = 0,
        OC_Error,
        OC_Warning_Level1,
        OC_Warning_Level2,
        OC_Warning_Level3,
        OC_Warning_Level4,
        OC_Dump
    };

    static void Output(OutputColor color, const char* format, ...);
};

}

#endif