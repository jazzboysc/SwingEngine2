// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SEPrimitive.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEPrimitive::SEPrimitive()
    :
    Type(PT_Unknown)
{
}
//----------------------------------------------------------------------------
SEPrimitive::~SEPrimitive()
{
    VB = 0;
    IB = 0;
}
//----------------------------------------------------------------------------