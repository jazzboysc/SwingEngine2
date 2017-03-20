// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_MaterialTemplate_H
#define Swing_MaterialTemplate_H

#include "SERenderingEngineLIB.h"
#include "SEReferencable.h"
#include "SETechnique.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 05/18/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEMaterialTemplate : public SEReferencable
{
public:
	SEMaterialTemplate();
	SEMaterialTemplate(SETechnique* _pTechnique);
	SEMaterialTemplate(std::initializer_list<SETechnique*> _techniques);
	~SEMaterialTemplate();

	void AddTechnique(SETechnique* technique);
	unsigned int GetTechniqueCount() const;
	SETechnique* GetTechnique(int i) const;

    void CreateDeviceResource(SEGPUDevice* device);

protected:
	std::vector<SETechnique*> mTechniques;
};

typedef SESmartPointer<SEMaterialTemplate> SEMaterialTemplatePtr;

}

#endif