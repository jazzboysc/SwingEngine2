// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_ScreenQuad_H
#define Swing_ScreenQuad_H

#include "SERenderingEngineLIB.h"
#include "SETriangleMesh.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 04/23/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEScreenQuad : public SETriangleMesh
{
public:
	SEScreenQuad(SEMaterial* material, SERTGICamera* camera = 0);
	virtual ~SEScreenQuad();

	// Implement base class interface.
	virtual void OnUpdateShaderConstants(int technique, int pass);
	virtual void OnGetShaderConstants();
};

typedef SESmartPointer<SEScreenQuad> SEScreenQuadPtr;

}

#endif