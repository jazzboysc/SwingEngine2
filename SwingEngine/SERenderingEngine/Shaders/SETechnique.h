// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_Technique_H
#define Swing_Technique_H

#include "SERenderingEngineLIB.h"
#include "SEPass.h"
#include "SEPassManager.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 05/18/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SETechnique : public SEPassManager
{
public:
	SETechnique();
	SETechnique(SEPass* _pPass);
	SETechnique(std::initializer_list<SEPass*> _passes);
	~SETechnique();
};

typedef SESmartPointer<SETechnique> SETechniquePtr;

}

#endif