// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_ComputeTask_H
#define Swing_ComputeTask_H

#include "SERenderingEngineLIB.h"
#include "SEPassManager.h"
#include "SEComputePass.h"
#include "SEBuffer.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 09/23/2013
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEComputeTask : public SEPassManager
{
public:
    SEComputeTask();
    virtual ~SEComputeTask();

    void CreateDeviceResource(SEGPUDevice* device);

    void DispatchCompute(unsigned int pass, unsigned int globalX, 
        unsigned int globalY, unsigned int globalZ);

    void DispatchComputeIndirect(unsigned int pass, 
        SEBuffer* indirectCommandBuffer, SEBufferView* indirectCommandBufferView, 
        void* indirect);

    void DispatchVertex(unsigned int pass, unsigned int threadCount);

    void DispatchVertexIndirect(unsigned int pass, 
        SEBuffer* indirectCommandBuffer, SEBufferView* indirectCommandBufferView, 
        void* indirect);

    virtual void OnGetShaderConstants(){};
    virtual void OnPreDispatch(unsigned int){};
    virtual void OnPostDispatch(unsigned int){};
};

typedef SESmartPointer<SEComputeTask> SEComputeTaskPtr;

}

#endif