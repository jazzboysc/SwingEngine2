// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_Material_H
#define Swing_Material_H

#include "SERenderingEngineLIB.h"
#include "SEReferencable.h"
#include "SEMaterialTemplate.h"
#include "SETechniqueInfo.h"
#include "SEGeometryAttributes.h"
#include "SETexture2D.h"

namespace Swing
{

class SERenderObject;
//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 05/18/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEMaterial : public SEReferencable
{
public:
	SEMaterial(SEMaterialTemplate* materialTemplate);
	~SEMaterial();

	void Apply(int techniqueNum, int passNum);
    void CreateDeviceResource(SEGPUDevice* device, 
        SEGeometryAttributes* geometryAttr);
	SEShaderProgram* GetProgram(int technique, int pass);

    void SetDiffuseMapName(const std::string& diffuseMapName);
    void CreateDiffuseMap(SEGPUDevice* device, 
        const std::string& diffuseMapName);
    SETexture2D* GetDiffuseMap();

protected:
	SEMaterialTemplatePtr mMaterialTemplate;

	friend class SERenderObject;
	SERenderObject* mRenderObject;

	std::vector<SETechniqueInfo*> mTechniqueInfo;

    std::string mDiffuseMapName;
    SETexture2DPtr mDiffuseMap;
};

typedef SESmartPointer<SEMaterial> SEMaterialPtr;

}

#endif