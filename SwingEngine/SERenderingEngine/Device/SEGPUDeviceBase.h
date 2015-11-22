// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_GPUDeviceBase_H
#define Swing_GPUDeviceBase_H

#include "SERenderingEngineLIB.h"
#include "SEReferencable.h"
#include "SEGPUResource.h"

namespace Swing
{

//----------------------------------------------------------------------------
#define SE_INSERT_GPU_DEVICE_BASE_FUNC(function, device) \
    _##function = (GPUDeviceBase##function)&##device::__##function
//----------------------------------------------------------------------------

struct SE_RENDERING_ENGINE_API SEGPUDeviceDescription
{
    unsigned int FramebufferWidth;
    unsigned int FramebufferHeight;
    bool EnableMSAA;
};

enum SEGPUDeviceVendor
{
    DV_Unknown = 0,
    DV_NVIDIA,
    DV_AMD
};

struct SE_RENDERING_ENGINE_API SEGPUDeviceInfo
{
    bool Valid;
    SEGPUDeviceVendor Vendor;
    unsigned int GPUTotalMemory;
    unsigned int GPUDedicatedMemory;
};

class SEGPUDeviceBase;
class SEGPUDeviceInspector;
class SEShader;
class SEShaderProgram;
class SEPassInfo;
class SEShaderUniform;
class SETexture;
class SEBuffer;
class SEBufferView;
class SEPixelBuffer;
class SETextureBuffer;
class SEFrameBuffer;
class SEPrimitive;
class SEGPUTimer;

class SECommandAllocator;
class SECommandList;
class SECommandQueue;
class SERenderCommandQueue;
class SERenderCommandAllocator;
class SERenderCommandList;
class SEComputeCommandQueue;
class SEComputeCommandAllocator;
class SEComputeCommandList;

class SEVector2f;
class SEVector3f;
class SEVector4f;
class SEMatrix4f;

struct SEShaderHandle;
struct SEShaderProgramHandle;
struct SEPassInfoHandle;
struct SEShaderUniformHandle;
struct SETextureHandle;
struct SEFBOHandle;
struct SEBufferHandle;
struct SEBufferViewHandle;
struct SEGPUTimerHandle;
struct SESamplerDesc;
struct SEPipelineStateBlock;
struct SEViewportState;
struct SECommandQueueHandle;
struct SECommandAllocatorHandle;
struct SECommandListHandle;

enum SEShaderProgramParameter;
enum SEBufferInternalFormat;
enum SEBufferFormat;
enum SEBufferComponentType;
enum SEBufferAccess;
enum SEBufferUsage;
enum SEPrimitiveType;

typedef void (SEGPUDeviceBase::*GPUDeviceBaseInitialize)(SEGPUDeviceDescription* deviceDesc);
typedef void (SEGPUDeviceBase::*GPUDeviceBaseTerminate)();
typedef void (SEGPUDeviceBase::*GPUDeviceBaseGetMaxAnisFilterLevel)(int* maxAnisFilterLevel);
typedef void (SEGPUDeviceBase::*GPUDeviceBaseSetAnisFilterLevel)(int maxAnisFilterLevel);
typedef SEShaderHandle* (SEGPUDeviceBase::*GPUDeviceBaseCreateShader)(SEShader* shader);
typedef void (SEGPUDeviceBase::*GPUDeviceBaseDeleteShader)(SEShader* shader);
typedef SECommandQueueHandle* (SEGPUDeviceBase::*GPUDeviceBaseCreateCommandQueue)(SECommandQueue* commandQueue);
typedef void (SEGPUDeviceBase::*GPUDeviceBaseDeleteCommandQueue)(SECommandQueue* commandQueue);
typedef SECommandAllocatorHandle* (SEGPUDeviceBase::*GPUDeviceBaseCreateCommandAllocator)(
    SECommandAllocator* commandAllocator, SECommandList* commandList);
typedef void (SEGPUDeviceBase::*GPUDeviceBaseDeleteCommandAllocator)(
    SECommandAllocator* commandAllocator, SECommandList* commandList);
typedef SECommandListHandle* (SEGPUDeviceBase::*GPUDeviceBaseCreateCommandList)(
    SECommandList* commandList, SECommandAllocator* commandAllocator);
typedef void (SEGPUDeviceBase::*GPUDeviceBaseDeleteCommandList)(
    SECommandList* commandList, SECommandAllocator* commandAllocator);

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/08/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEGPUDeviceBase : public SEReferencable
{
public:
    ~SEGPUDeviceBase();

protected:
    // Abstract base class.
    SEGPUDeviceBase();

public:
    inline 	void Initialize(SEGPUDeviceDescription* deviceDesc);
    inline 	void Terminate();

    // Device query stuff.
    inline 	void GetMaxAnisFilterLevel(int* maxAnisFilterLevel);
    inline 	void SetAnisFilterLevel(int maxAnisFilterLevel);

    // Shader stuff.
    inline 	SEShaderHandle* CreateShader(SEShader* shader);
    inline 	void DeleteShader(SEShader* shader);

    // Command queue stuff.
    inline 	SECommandQueueHandle* CreateCommandQueue(SECommandQueue* commandQueue);
    inline  void DeleteCommandQueue(SECommandQueue* commandQueue);

    // Command allocator stuff.
    inline  SECommandAllocatorHandle* CreateCommandAllocator(
        SECommandAllocator* commandAllocator, SECommandList* commandList);
    inline  void DeleteCommandAllocator(
        SECommandAllocator* commandAllocator, SECommandList* commandList);

    // Command list stuff.
    inline SECommandListHandle* CreateCommandList(SECommandList* commandList, 
        SECommandAllocator* commandAllocator);
    inline void DeleteCommandList(SECommandList* commandList, 
        SECommandAllocator* commandAllocator);

protected:
    GPUDeviceBaseInitialize                           _Initialize;
    GPUDeviceBaseTerminate                            _Terminate;
    GPUDeviceBaseGetMaxAnisFilterLevel				  _GetMaxAnisFilterLevel;
    GPUDeviceBaseSetAnisFilterLevel					  _SetAnisFilterLevel;
    GPUDeviceBaseCreateShader                         _CreateShader;
    GPUDeviceBaseDeleteShader                         _DeleteShader;

    GPUDeviceBaseCreateCommandQueue                   _CreateCommandQueue;
    GPUDeviceBaseDeleteCommandQueue                   _DeleteCommandQueue;
    GPUDeviceBaseCreateCommandAllocator               _CreateCommandAllocator;
    GPUDeviceBaseDeleteCommandAllocator               _DeleteCommandAllocator;
    GPUDeviceBaseCreateCommandList                    _CreateCommandList;
    GPUDeviceBaseDeleteCommandList                    _DeleteCommandList;

    SERenderCommandQueue*     mDefaultRenderCommandQueue;
    SERenderCommandAllocator* mDefaultRenderCommandAllocator;
    SERenderCommandList*      mDefaultRenderCommandList;

protected:
    // Device capabilities.
    int mMaxTextureArrayLayer;

    SEGPUDeviceDescription mDeviceDesc;
    SEGPUDeviceInfo mDeviceInfo;
    SEGPUDeviceInspector* mInspector;
};

typedef SESmartPointer<SEGPUDeviceBase> SEGPUDeviceBasePtr;

#include "SEGPUDeviceBase.inl"

}

#endif