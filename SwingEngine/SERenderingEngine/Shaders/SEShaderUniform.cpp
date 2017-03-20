// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERenderingEnginePCH.h"
#include "SEShaderUniform.h"
#include "SEGPUDevice.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEShaderUniform::SEShaderUniform()
{
    mType = UT_Unknown;
    mUniformHandle = 0;
}
//----------------------------------------------------------------------------
SEShaderUniform::~SEShaderUniform()
{
    SE_DELETE mUniformHandle;
    mUniformHandle = 0;
}
//----------------------------------------------------------------------------
void SEShaderUniform::SetValue(int value)
{
	this->mUniformHandle->Device->SetUniformValueInt(this, value);
}
//----------------------------------------------------------------------------
void SEShaderUniform::SetValue(unsigned int value)
{
    this->mUniformHandle->Device->SetUniformValueUInt(this, value);
}
//----------------------------------------------------------------------------
void SEShaderUniform::SetValue(float value)
{
	this->mUniformHandle->Device->SetUniformValueFloat(this, value);
}
//----------------------------------------------------------------------------
void SEShaderUniform::SetValue(float value[2])
{
	this->mUniformHandle->Device->SetUniformValueFloat2(this, value);
}
//----------------------------------------------------------------------------
void SEShaderUniform::SetValue(const SEMatrix4f& value)
{
	this->mUniformHandle->Device->SetUniformValueMat4(this, &value);
}
//----------------------------------------------------------------------------
void SEShaderUniform::SetValue(const SEVector3f& value)
{
	this->mUniformHandle->Device->SetUniformValueVec3(this, &value);
}
//----------------------------------------------------------------------------
void SEShaderUniform::SetValue(const SEVector4f& value)
{
	this->mUniformHandle->Device->SetUniformValueVec4(this, &value);
}
//----------------------------------------------------------------------------