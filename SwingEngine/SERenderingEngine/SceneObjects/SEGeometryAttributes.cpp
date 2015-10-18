// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SEGeometryAttributes.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEGeometryAttributes::SEGeometryAttributes()
{
    Prim = 0;
	HasNormal = false;
	HasTCoord = false;
	VertexComponentCount = false;
}
//----------------------------------------------------------------------------
SEGeometryAttributes::~SEGeometryAttributes()
{
}
//----------------------------------------------------------------------------