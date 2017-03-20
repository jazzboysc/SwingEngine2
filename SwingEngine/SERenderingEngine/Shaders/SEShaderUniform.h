// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_ShaderUniform_H
#define Swing_ShaderUniform_H

#include "SERenderingEngineLIB.h"
#include "SEGPUResource.h"

#include <string>

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 12/09/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEShaderUniform
{
public:
    SEShaderUniform();
    ~SEShaderUniform();

    enum SEUniformType
    {
        UT_Unknown = 0,
        UT_SystemPerFrame,
        UT_SystemPerGroup,
        UT_SystemPerObject,
        UT_UserPerFrame,
        UT_UserPerGroup,
        UT_UserPerObject,
        UT_Max
    };

    void SetValue(int value);
    void SetValue(unsigned int value);
    void SetValue(float value);
    void SetValue(float value[2]);
    void SetValue(const SEMatrix4f& value);
	void SetValue(const SEVector3f& value);
	void SetValue(const SEVector4f& value);

public:
    std::string mName;
    SEUniformType mType;
    SEShaderUniformHandle* mUniformHandle;
};

typedef SESmartPointer<SEShaderUniform> SEShaderUniformPtr;

}

#endif