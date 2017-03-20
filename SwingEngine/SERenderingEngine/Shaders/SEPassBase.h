// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_PassBase_H
#define Swing_PassBase_H

#include "SERenderingEngineLIB.h"
#include "SEReferencable.h"
#include "SEShaderProgram.h"
#include "SEPipelineStateBlock.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 08/09/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEPassBase : public SEReferencable
{
public:
    SEPassBase(const SEShaderProgramInfo& programInfo);
    SEPassBase(SEShaderProgram* shaderProgram);
    virtual ~SEPassBase();

    SEShaderProgram* GetShaderProgram();
    void CreateDeviceResource(SEGPUDevice* device);

    void SetPipelineStateBlock(SEPipelineStateBlock* psb);
    SEPipelineStateBlock* GetPipelineStateBlock() const;

protected:
    SEShaderProgramPtr mShaderProgram;
    SEPipelineStateBlockPtr mPSB;
};

typedef SESmartPointer<SEPassBase> SEPassBasePtr;

}

#endif