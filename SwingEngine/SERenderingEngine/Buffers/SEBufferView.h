// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_BufferView_H
#define Swing_BufferView_H

#include "SERenderingEngineLIB.h"
#include "SEReferencable.h"
#include "SEGPUResource.h"

namespace Swing
{

enum SEBufferType
{
    BT_Typeless = -1,
    BT_AtomicCounter,
    BT_DispatchIndirect,
    BT_DrawIndirect,
    BT_Pixel,
    BT_Structured,
    BT_TextureBuffer,
    BT_Uniform,
    BT_Vertex,
    BT_Index,
    BufferType_Max
};

struct SE_RENDERING_ENGINE_API SEBufferViewDesc
{
    SEBufferType Type;
};

class SEBufferBase;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 04/09/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEBufferView : public SEReferencable
{
public:
    SEBufferView(const SEBufferViewDesc& viewDesc);
    ~SEBufferView();

    inline SEBufferType GetBufferType() const { return mViewDesc.Type; }

    void CreateDeviceResource(SEGPUDevice* device, SEBufferBase* bufferBase);

private:
    SEBufferViewDesc mViewDesc;
    SEBufferViewHandle* mBufferViewHandle;
    SEBufferBase* mBufferBase;
};

typedef SESmartPointer<SEBufferView> SEBufferViewPtr;

}

#endif