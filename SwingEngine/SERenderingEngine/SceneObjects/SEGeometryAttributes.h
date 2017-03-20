// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_GeometryAttributes_H
#define Swing_GeometryAttributes_H

#include "SERenderingEngineLIB.h"
#include "SEPrimitive.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 08/09/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEGeometryAttributes
{
public:
	SEGeometryAttributes();
	~SEGeometryAttributes();

    SEPrimitive* Prim;
	bool HasNormal;
	bool HasTCoord;
	int VertexComponentCount;
};

}

#endif