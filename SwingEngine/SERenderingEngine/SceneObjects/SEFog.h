// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_Fog_H
#define Swing_Fog_H

#include "SERenderingEngineLIB.h"
#include "SEReferencable.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/15/2013
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEFog : public SEReferencable
{
public:
	SEFog();
    ~SEFog();

	SEVector3f Color;
	SEVector2f Range;
	bool IsExponential;
};

typedef SESmartPointer<SEFog> SEFogPtr;

}

#endif
