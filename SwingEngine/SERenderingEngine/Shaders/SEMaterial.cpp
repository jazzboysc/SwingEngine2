// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SEMaterial.h"
#include "SERenderObject.h"
#include "SETextureManager.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEMaterial::SEMaterial(SEMaterialTemplate* materialTemplate)
{
	mRenderObject = 0;
	mMaterialTemplate = materialTemplate;
}
//----------------------------------------------------------------------------
SEMaterial::~SEMaterial()
{
	mMaterialTemplate = 0;
    mDiffuseMap = 0;
}
//----------------------------------------------------------------------------
void SEMaterial::Apply(int techniqueNum, int passNum)
{
	SETechnique* tech = mMaterialTemplate->GetTechnique(techniqueNum);
    SE_ASSERT( tech );

    SERenderPass* pass = (SERenderPass*)tech->GetPass(passNum);
    SE_ASSERT( pass );

	SERenderPassInfo* renderPassInfo = mTechniqueInfo[techniqueNum]->GetPassInfo(passNum);
	SE_ASSERT( renderPassInfo );
    
	renderPassInfo->Enable();
    pass->Enable();

    mRenderObject->OnEnableBuffers();
    mRenderObject->OnUpdateShaderConstants(techniqueNum, passNum);
    mRenderObject->OnRender(pass, renderPassInfo);
    mRenderObject->OnDisableBuffers();

    pass->Disable();
	renderPassInfo->Disable();
}
//----------------------------------------------------------------------------
void SEMaterial::CreateDeviceResource(SEGPUDevice* device, 
    SEGeometryAttributes* geometryAttr)
{
	mMaterialTemplate->CreateDeviceResource(device);

	unsigned int tcount = mMaterialTemplate->GetTechniqueCount();
	mTechniqueInfo.reserve(tcount);
	for( unsigned int i = 0; i < tcount; ++i )
	{
		SETechnique* technique = mMaterialTemplate->GetTechnique((int)i);
		SETechniqueInfo* techInfo = SE_NEW SETechniqueInfo();
		techInfo->CreateRenderPassInfo(device, technique, geometryAttr);
		mTechniqueInfo.push_back(techInfo);
	}

    if( mDiffuseMapName != "" )
    {
        CreateDiffuseMap(device, mDiffuseMapName);
    }
}
//----------------------------------------------------------------------------
SEShaderProgram* SEMaterial::GetProgram(int technique, int pass)
{
	SETechnique* t = mMaterialTemplate->GetTechnique(technique);
	SERenderPass* p = (SERenderPass*)t->GetPass(pass);
	SEShaderProgram* program = p->GetShaderProgram();
	return program;
}
//----------------------------------------------------------------------------
void SEMaterial::SetDiffuseMapName(const std::string& diffuseMapName)
{
    mDiffuseMapName = diffuseMapName;
}
//----------------------------------------------------------------------------
void SEMaterial::CreateDiffuseMap(SEGPUDevice* device, 
    const std::string& diffuseMapName)
{
    if( mDiffuseMap )
    {
        SE_ASSERT(false);
        return;
    }

    mDiffuseMap = SETextureManager::GetInstance()->FindOrCreateTexture2D(device, 
        diffuseMapName);
}
//----------------------------------------------------------------------------
SETexture2D* SEMaterial::GetDiffuseMap()
{
    return mDiffuseMap;
}
//----------------------------------------------------------------------------