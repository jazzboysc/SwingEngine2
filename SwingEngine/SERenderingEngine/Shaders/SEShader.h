// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_Shader_H
#define Swing_Shader_H

#include "SERenderingEngineLIB.h"
#include "SEReferencable.h"
#include "SEGPUDevice.h"
#include <string>

namespace Swing
{

enum SEShaderType
{
    ST_Unknown = -1,
    ST_Vertex,
    ST_Fragment,
    ST_Geometry,
    ST_Compute,
    ST_TessellationControl,
    ST_TessellationEvaluation,
    ShaderType_Max
};

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 09/14/2013
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEShader : public SEReferencable
{
public:
    SEShader(SEShaderType type, const std::string& shaderFileName);
    virtual ~SEShader();

	void CreateDeviceResource(SEGPUDevice* device);
    SEShaderHandle* GetShaderHandle() const;
    std::string GetShaderFileName() const;
    SEShaderType GetType() const;
    const std::string& GetSource() const;

protected:
	bool LoadFromFile(const std::string& shaderFileName);

    std::string mShaderFileName;
	std::string mShaderSource;
    SEShaderType mType;
    SEShaderHandle*  mShaderHandle;
};

typedef SESmartPointer<SEShader> SEShaderPtr;

}

#endif
