// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SEScreenQuad.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEScreenQuad::SEScreenQuad(SEMaterial* material, SERTGICamera* camera)
	:
	SETriangleMesh(material, camera)
{
}
//----------------------------------------------------------------------------
SEScreenQuad::~SEScreenQuad()
{
}
//----------------------------------------------------------------------------
void SEScreenQuad::OnUpdateShaderConstants(int, int)
{
	// Nothing to do.
}
//----------------------------------------------------------------------------
void SEScreenQuad::OnGetShaderConstants()
{
	// Nothing to do.
}
//----------------------------------------------------------------------------