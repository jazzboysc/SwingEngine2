// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERenderingEnginePCH.h"
#include "SEGPUDeviceInspector.h"
#include "SETerminal.h"
#include "SETexture.h"
#include "SEBuffer.h"
#include "SEFrameBuffer.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEGPUDeviceInspector::SEGPUDeviceInspector(SEGPUDevice* device)
{
    mGPUDevice = device;
    mGPUDevice->SetInspector(this);
}
//----------------------------------------------------------------------------
SEGPUDeviceInspector::~SEGPUDeviceInspector()
{
    mGPUDevice = 0;
}
//----------------------------------------------------------------------------
void SEGPUDeviceInspector::OnDeviceMemoryOperation(SEDeviceMemOp* op)
{
    mMemOps.push_back(*op);
}
//----------------------------------------------------------------------------
void SEGPUDeviceInspector::OnDetach()
{
}
//----------------------------------------------------------------------------
void SEGPUDeviceInspector::DumpDeviceMemoryOperation()
{
    for( int i = 0; i < (int)mMemOps.size(); ++i )
    {
        SEDeviceMemOp& op = mMemOps[i];
        std::string type;
        if( dynamic_cast<SETexture*>(op.Resident) )
        {
            type = "SETexture";
        }
        else if( dynamic_cast<SEBuffer*>(op.Resident) )
        {
            type = "SEBuffer";
        }
        else if( dynamic_cast<SEFrameBuffer*>(op.Resident) )
        {
            type = "SEFrameBuffer";
        }
        else
        {
            type = "Unknown";
        }

        SETerminal::Output(SETerminal::OC_Dump, 
            "Resident Type: %s Device Memory Used: %d\n", type.c_str(), 
            op.DeviceMemPrint);
    }
}
//----------------------------------------------------------------------------