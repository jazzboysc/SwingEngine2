// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SEFog.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEFog::SEFog()
	:
	Color(1.0f, 1.0f, 1.0f),
	Range(0.0f, 1.0f),
	IsExponential(false)
{
}
//----------------------------------------------------------------------------
SEFog::~SEFog()
{
}
//----------------------------------------------------------------------------