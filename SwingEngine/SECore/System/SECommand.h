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

#ifndef Swing_Command_H
#define Swing_Command_H

#include "SECoreLIB.h"
#include "SESystem.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20080804
//----------------------------------------------------------------------------
class SE_CORE_API SECommand
{
public:
    SECommand(int iCount, char** apcArgument);
    SECommand(char* acCmdline);
    ~SECommand(void);

    // return value is index of first excess argument
    int ExcessArguments(void);

    // Set bounds for numerical arguments.  If bounds are required, they must
    // be set for each argument.
    SECommand& Min(double dValue);
    SECommand& Max(double dValue);
    SECommand& Inf(double dValue);
    SECommand& Sup(double dValue);

    // The return value of the following methods is the option index within
    // the argument array.

    // Use the boolean methods for options which take no argument, for
    // example in
    //           myprogram -debug -x 10 -y 20 filename
    // the option -debug has no argument.

    int Boolean(char* acName);  // returns existence of option
    int Boolean(char* acName, bool& rbValue);
    int Integer(char* acName, int& riValue);
    int Float(char* acName, float& rfValue);
    int Double(char* acName, double& rdValue);
    int String(char* acName, char*& racValue);
    int Filename(char*& racName);

    // last error reporting
    const char* GetLastError(void);

protected:
    // constructor support
    void Initialize(void);

    // command line information
    int m_iCount;       // number of arguments
    char** m_apcArgument;  // argument list (array of strings)
    char* m_acCmdline;     // argument list (single string)
    bool* m_abUsed;        // keeps track of arguments already processed

    // parameters for bounds checking
    double m_dSmall;   // lower bound for numerical argument (min or inf)
    double m_dLarge;   // upper bound for numerical argument (max or sup)
    bool m_bMinSet;    // if true, compare:  small <= arg
    bool m_bMaxSet;    // if true, compare:  arg <= large
    bool m_bInfSet;    // if true, compare:  small < arg
    bool m_bSupSet;    // if true, compare:  arg < large

    // last error strings
    const char* m_acLastError;
    static char ms_acOptionNotFound[];
    static char ms_acArgumentRequired[];
    static char ms_acArgumentOutOfRange[];
    static char ms_acFilenameNotFound[];
};

}

#endif
