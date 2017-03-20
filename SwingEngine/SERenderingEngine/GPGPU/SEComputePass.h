// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_ComputePass_H
#define Swing_ComputePass_H

#include "SERenderingEngineLIB.h"
#include "SEPassBase.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 09/23/2013
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEComputePass : public SEPassBase
{
public:
    SEComputePass(const SEShaderProgramInfo& programInfo);
    SEComputePass(SEShaderProgram* computeProgram);
    ~SEComputePass();

    inline bool IsVertexPass() const { return mIsVertexPass; }

protected:
    SEComputePass();

    bool mIsVertexPass;
};

typedef SESmartPointer<SEComputePass> SEComputePassPtr;

}

#endif