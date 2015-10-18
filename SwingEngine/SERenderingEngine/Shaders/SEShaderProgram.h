// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_ShaderProgram_H
#define Swing_ShaderProgram_H

#include "SERenderingEngineLIB.h"
#include "SEReferencable.h"
#include "SEVertexShader.h"
#include "SEFragmentShader.h"
#include "SEGeometryShader.h"
#include "SEComputeShader.h"
#include "SETessellationControlShader.h"
#include "SETessellationEvaluationShader.h"
#include "SEShaderUniform.h"

#include <vector>

namespace Swing
{

enum SEShaderStage
{
    SS_Unknown = 0,
    SS_Vertex = 1,
    SS_Fragment = 2,
    SS_Geometry = 4,
    SS_Compute = 8,
    SS_TessellationControl = 16,
    SS_TessellationEvaluation = 32
};

enum SEShaderProgramParameter
{
    SPP_Geometry_Vertices_Out = 0,
    SPP_Max
};

struct SE_RENDERING_ENGINE_API SEShaderProgramParameterValue
{
    SEShaderProgramParameter Name;
    int Value;

    SEShaderProgramParameterValue(SEShaderProgramParameter name, int value)
    {
        Name = name;
        Value = value;
    }
};

struct SE_RENDERING_ENGINE_API SEShaderProgramInfo
{
    std::string VShaderFileName;
    std::string FShaderFileName;
    std::string GShaderFileName;
    std::string CShaderFileName;
    std::string TCShaderFileName;
    std::string TEShaderFileName;
    unsigned char ShaderStageFlag;
    std::vector<SEShaderProgramParameterValue> Parameters;

    SEShaderProgramInfo();

    SEShaderProgramInfo& operator << (const char* _shaderName);
};

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 09/14/2013
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEShaderProgram : public SEReferencable
{
public:
    SEShaderProgram(const SEShaderProgramInfo& programInfo);
	SEShaderProgram(SEVertexShader* vShader, SEFragmentShader* fShader);
	virtual ~SEShaderProgram();

	void CreateDeviceResource(SEGPUDevice* device);

    void GetUniformLocation(SEShaderUniform* dstUniform, const char* name);

	SEShaderProgramHandle* GetProgramHandle() const;
	SEVertexShader* GetVertexShader() const;
    SEFragmentShader* GetFragmentShader() const;
    SEGeometryShader* GetGeometryShader() const;
    SEComputeShader* GetComputeShader() const;
    SETessellationControlShader* GetTessellationControlShader() const;
    SETessellationEvaluationShader* GetTessellationEvaluationShader() const;

    bool IsTessellationEnabled() const;
    bool IsComputeEnabled() const;
    bool IsVertexEnabled() const;

	void Enable();
	void Disable();

protected:
    SEShaderProgramInfo mProgramInfo;

	SEVertexShaderPtr mVertexShader;
	SEFragmentShaderPtr mFragmentShader;
    SEGeometryShaderPtr mGeometryShader;
    SEComputeShaderPtr mComputeShader;
    SETessellationControlShaderPtr mTessellationControlShader;
    SETessellationEvaluationShaderPtr mTessellationEvaluationShader;

	SEShaderProgramHandle* mProgramHandle;
};

typedef SESmartPointer<SEShaderProgram> SEShaderProgramPtr;

}

#endif