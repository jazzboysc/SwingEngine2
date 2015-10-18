// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SEMaterialTemplate.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEMaterialTemplate::SEMaterialTemplate()
{
}

SEMaterialTemplate::SEMaterialTemplate(SETechnique* _pTechnique)
{
	AddTechnique(_pTechnique);
}
//----------------------------------------------------------------------------
SEMaterialTemplate::SEMaterialTemplate(
	std::initializer_list<SETechnique*> _techniques)
{
	for (auto i : _techniques)
	{
		AddTechnique(i);
	}
}

//----------------------------------------------------------------------------
SEMaterialTemplate::~SEMaterialTemplate()
{
	for( int i = 0; i < (int)mTechniques.size(); ++i )
	{
		SE_DELETE mTechniques[i];
	}
    mTechniques.clear();
}
//----------------------------------------------------------------------------
void SEMaterialTemplate::AddTechnique(SETechnique* technique)
{
	SE_ASSERT( technique );
	mTechniques.push_back(technique);
}
//----------------------------------------------------------------------------
unsigned int SEMaterialTemplate::GetTechniqueCount() const
{
	return (unsigned int)mTechniques.size();
}
//----------------------------------------------------------------------------
SETechnique* SEMaterialTemplate::GetTechnique(int i) const
{
	SE_ASSERT( i < (int)mTechniques.size() );

	return mTechniques[i];
}
//----------------------------------------------------------------------------
void SEMaterialTemplate::CreateDeviceResource(SEGPUDevice* device)
{
	for( int i = 0; i < (int)mTechniques.size(); ++i )
	{
		mTechniques[i]->CreateDeviceResource(device);
	}
}
//----------------------------------------------------------------------------