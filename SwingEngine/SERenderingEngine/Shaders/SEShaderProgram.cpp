// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SEShaderProgram.h"

#include <algorithm>

using namespace Swing;

//----------------------------------------------------------------------------
SEShaderProgramInfo::SEShaderProgramInfo()
{
    ShaderStageFlag = SEShaderStage::SS_Unknown;
}
//----------------------------------------------------------------------------
SEShaderProgramInfo& SEShaderProgramInfo::operator << (const char* _shaderName)
{
    std::string shaderName = _shaderName;
    std::transform(shaderName.begin(), shaderName.end(), shaderName.begin(),
        ::tolower);
    auto size = shaderName.size();
    auto fileExt = std::string(&shaderName.c_str()[size - 4]);
    if( fileExt == "vert" )
    {
        VShaderFileName = shaderName;
        ShaderStageFlag |= SEShaderStage::SS_Vertex;
    }
    else if( fileExt == "frag" )
    {
        FShaderFileName = shaderName;
        ShaderStageFlag |= SEShaderStage::SS_Fragment;
    }
    else if( fileExt == "geom" )
    {
        GShaderFileName = shaderName;
        ShaderStageFlag |= SEShaderStage::SS_Geometry;
    }
    else if( fileExt == "ctrl" )
    {
        TCShaderFileName = shaderName;
        ShaderStageFlag |= SEShaderStage::SS_TessellationControl;
    }
    else if( fileExt == "eval" )
    {
        TEShaderFileName = shaderName;
        ShaderStageFlag |= SEShaderStage::SS_TessellationEvaluation;
    }
    else if( fileExt == "comp" )
    {
        CShaderFileName = shaderName;
        ShaderStageFlag |= SEShaderStage::SS_Compute;
    }
    return *this;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
SEShaderProgram::SEShaderProgram(const SEShaderProgramInfo& programInfo)
	:
	mProgramHandle(0)
{
    if( programInfo.ShaderStageFlag & SEShaderStage::SS_Vertex )
    {
        mVertexShader = SE_NEW SEVertexShader(programInfo.VShaderFileName);
    }

    if( programInfo.ShaderStageFlag & SEShaderStage::SS_Fragment )
    {
        mFragmentShader = SE_NEW SEFragmentShader(programInfo.FShaderFileName);
    }

    if( programInfo.ShaderStageFlag & SEShaderStage::SS_Geometry )
    {
        mGeometryShader = SE_NEW SEGeometryShader(programInfo.GShaderFileName);
    }

    if( programInfo.ShaderStageFlag & SEShaderStage::SS_Compute )
    {
        mComputeShader = SE_NEW SEComputeShader(programInfo.CShaderFileName);
    }

    if( programInfo.ShaderStageFlag & SEShaderStage::SS_TessellationControl )
    {
        mTessellationControlShader = SE_NEW SETessellationControlShader(
            programInfo.TCShaderFileName);
    }

    if( programInfo.ShaderStageFlag & SEShaderStage::SS_TessellationEvaluation )
    {
        mTessellationEvaluationShader = SE_NEW SETessellationEvaluationShader(
            programInfo.TEShaderFileName);
    }
    mProgramInfo = programInfo;
}
//----------------------------------------------------------------------------
SEShaderProgram::SEShaderProgram(SEVertexShader* vShader, SEFragmentShader* fShader)
	:
	mProgramHandle(0)
{
	SE_ASSERT( vShader && fShader );
	mVertexShader = vShader;
	mFragmentShader = fShader;
}
//----------------------------------------------------------------------------
SEShaderProgram::~SEShaderProgram()
{
	mVertexShader = 0;
	mFragmentShader = 0;
    mGeometryShader = 0;
    mComputeShader = 0;
    mTessellationControlShader = 0;
    mTessellationEvaluationShader = 0;

    if( mProgramHandle )
    {
        mProgramHandle->Device->DeleteProgram(this);
        SE_DELETE mProgramHandle;
        mProgramHandle = 0;
    }
}
//----------------------------------------------------------------------------
void SEShaderProgram::CreateDeviceResource(SEGPUDevice* device)
{
	if( mProgramHandle )
	{
		return;
	}

    mProgramHandle = device->CreateProgram(this);
    for( int i = 0; i < (int)mProgramInfo.Parameters.size(); ++i )
    {
        mProgramHandle->Device->SetProgramParameterInt(this, 
            mProgramInfo.Parameters[i].Name, 
            mProgramInfo.Parameters[i].Value);
    }
}
//----------------------------------------------------------------------------
void SEShaderProgram::GetUniformLocation(SEShaderUniform* dstUniform, 
    const char* name)
{
    this->mProgramHandle->Device->GetUniformLocation(this, dstUniform, name);
}
//----------------------------------------------------------------------------
SEShaderProgramHandle* SEShaderProgram::GetProgramHandle() const
{
	return mProgramHandle;
}
//----------------------------------------------------------------------------
SEVertexShader* SEShaderProgram::GetVertexShader() const
{
	return mVertexShader;
}
//----------------------------------------------------------------------------
SEFragmentShader* SEShaderProgram::GetFragmentShader() const
{
	return mFragmentShader;
}
//----------------------------------------------------------------------------
SEGeometryShader* SEShaderProgram::GetGeometryShader() const
{
    return mGeometryShader;
}
//----------------------------------------------------------------------------
SEComputeShader* SEShaderProgram::GetComputeShader() const
{
    return mComputeShader;
}
//----------------------------------------------------------------------------
SETessellationControlShader* SEShaderProgram::GetTessellationControlShader() const
{
    return mTessellationControlShader;
}
//----------------------------------------------------------------------------
SETessellationEvaluationShader* SEShaderProgram::GetTessellationEvaluationShader(
    ) const
{
    return mTessellationEvaluationShader;
}
//----------------------------------------------------------------------------
void SEShaderProgram::Enable()
{
    mProgramHandle->Device->EnableProgram(this);
}
//----------------------------------------------------------------------------
void SEShaderProgram::Disable()
{
    mProgramHandle->Device->DisableProgram(this);
}
//----------------------------------------------------------------------------
bool SEShaderProgram::IsTessellationEnabled() const
{
    if( mProgramInfo.ShaderStageFlag & SEShaderStage::SS_TessellationEvaluation )
    {
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------
bool SEShaderProgram::IsComputeEnabled() const
{
    if( mProgramInfo.ShaderStageFlag & SEShaderStage::SS_Compute )
    {
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------
bool SEShaderProgram::IsVertexEnabled() const
{
    if( mProgramInfo.ShaderStageFlag & SEShaderStage::SS_Vertex )
    {
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------