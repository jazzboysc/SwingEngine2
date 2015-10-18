// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_ThreadType_H
#define Swing_ThreadType_H

#include "SECoreLIB.h"
#include "SEPlatforms.h"

#if defined(SE_IS_WINDOWS)
//----------------------------------------------------------------------------
namespace Swing
{
    typedef void* SEThreadType;
}
//----------------------------------------------------------------------------
#else
#error Other platforms not yet implemented.
#endif

#endif
