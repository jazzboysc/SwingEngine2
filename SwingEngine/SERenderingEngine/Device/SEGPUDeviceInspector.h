// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_GPUDeviceInspector_H
#define Swing_GPUDeviceInspector_H

#include "SERenderingEngineLIB.h"
#include "SEReferencable.h"
#include "SEGPUDevice.h"
#include "SEGPUDeviceResident.h"

#include <vector>

namespace Swing
{

struct SE_RENDERING_ENGINE_API SEDeviceMemOp
{
    SEGPUDeviceResident* Resident;
    int DeviceMemPrint;
};

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 07/23/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEGPUDeviceInspector : public SEReferencable
{
public:
    SEGPUDeviceInspector(SEGPUDevice* device);
    virtual ~SEGPUDeviceInspector();

    virtual void OnDeviceMemoryOperation(SEDeviceMemOp* op);
    virtual void OnDetach();

    void DumpDeviceMemoryOperation();

protected:
    SEGPUDevicePtr mDevice;
    std::vector<SEDeviceMemOp> mMemOps;
};

typedef SESmartPointer<SEGPUDeviceInspector> SEGPUDeviceInspectorPtr;

}

#endif