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
class SERenderPassInfo;
class SEGeometryAttributes;
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
class SERenderCommandAllocator;
class SERenderCommandList;
class SERenderCommandQueue;
class SEComputeCommandAllocator;
class SEComputeCommandList;
class SEComputeCommandQueue;
class SERootSignature;

class SEVector2f;
class SEVector3f;
class SEVector4f;
class SEMatrix4f;

struct SEShaderHandle;
struct SEShaderProgramHandle;
struct SERenderPassInfoHandle;
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
struct SERootSignatureHandle;
struct SERenderPassTargetsInfo;

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

// Shader.
typedef SEShaderHandle* (SEGPUDeviceBase::*GPUDeviceBaseCreateShader)(SEShader* shader);
typedef void (SEGPUDeviceBase::*GPUDeviceBaseDeleteShader)(SEShader* shader);

// Render pass info.
typedef SERenderPassInfoHandle* (SEGPUDeviceBase::*GPUDeviceBaseCreateRenderPassInfo)(
    SERenderPassInfo* renderPassInfo, SEShaderProgram* program, SEGeometryAttributes* geometryAttr,
    SEPipelineStateBlock* psb, SERootSignature* rootSignature, SERenderPassTargetsInfo* targetsInfo);
typedef void (SEGPUDeviceBase::*GPUDeviceBaseDeleteRenderPassInfo)(SERenderPassInfo* renderPassInfo);
typedef void (SEGPUDeviceBase::*GPUDeviceBaseEnableRenderPassInfo)(SERenderPassInfo* renderPassInfo);
typedef void (SEGPUDeviceBase::*GPUDeviceBaseDisableRenderPassInfo)(SERenderPassInfo* renderPassInfo);

// Command queue.
typedef SECommandQueueHandle* (SEGPUDeviceBase::*GPUDeviceBaseCreateCommandQueue)(SECommandQueue* commandQueue);
typedef void (SEGPUDeviceBase::*GPUDeviceBaseDeleteCommandQueue)(SECommandQueue* commandQueue);

// Command allocator.
typedef SECommandAllocatorHandle* (SEGPUDeviceBase::*GPUDeviceBaseCreateCommandAllocator)(
    SECommandAllocator* commandAllocator, SECommandList* commandList);
typedef void (SEGPUDeviceBase::*GPUDeviceBaseDeleteCommandAllocator)(
    SECommandAllocator* commandAllocator, SECommandList* commandList);

// Command list.
typedef SECommandListHandle* (SEGPUDeviceBase::*GPUDeviceBaseCreateCommandList)(
    SECommandList* commandList, SECommandAllocator* commandAllocator);
typedef void (SEGPUDeviceBase::*GPUDeviceBaseDeleteCommandList)(
    SECommandList* commandList, SECommandAllocator* commandAllocator);
typedef void (SEGPUDeviceBase::*GPUDeviceBaseResetRenderCommandList)(
    SERenderCommandList* renderCommandList, SERenderPassInfo* renderPassInfo);
typedef void (SEGPUDeviceBase::*GPUDeviceBaseCloseRenderCommandList)(
    SERenderCommandList* renderCommandList);
typedef void (SEGPUDeviceBase::*GPUDeviceBaseRenderCommandListSetRootSignature)(
    SERenderCommandList* renderCommandList, SERootSignature* rootSignature);
typedef void (SEGPUDeviceBase::*GPUDeviceBaseRenderCommandListSetViewport)(
    SERenderCommandList* renderCommandList, SEViewportState* srcViewport);

// Root signature.
typedef SERootSignatureHandle* (SEGPUDeviceBase::*GPUDeviceBaseCreateRootSignature)(
    SERootSignature* rootSignature);
typedef void (SEGPUDeviceBase::*GPUDeviceBaseDeleteRootSignature)(
    SERootSignature* rootSignature);

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

    // Render pass info stuff.
    inline 	SERenderPassInfoHandle* CreateRenderPassInfo(SERenderPassInfo* renderPassInfo,
        SEShaderProgram* program, SEGeometryAttributes* geometryAttr,
        SEPipelineStateBlock* psb, SERootSignature* rootSignature, 
        SERenderPassTargetsInfo* targetsInfo);
    inline 	void DeleteRenderPassInfo(SERenderPassInfo* renderPassInfo);
    inline void EnableRenderPassInfo(SERenderPassInfo* renderPassInfo);
    inline void DisableRenderPassInfo(SERenderPassInfo* renderPassInfo);

    // Command queue stuff.
    inline 	SECommandQueueHandle* CreateCommandQueue(SECommandQueue* commandQueue);
    inline  void DeleteCommandQueue(SECommandQueue* commandQueue);

    // Command allocator stuff.
    inline  SECommandAllocatorHandle* CreateCommandAllocator(
        SECommandAllocator* commandAllocator, SECommandList* commandList);
    inline  void DeleteCommandAllocator(
        SECommandAllocator* commandAllocator, SECommandList* commandList);

    // Command list stuff.
    inline  SECommandListHandle* CreateCommandList(SECommandList* commandList, 
        SECommandAllocator* commandAllocator);
    inline  void DeleteCommandList(SECommandList* commandList, 
        SECommandAllocator* commandAllocator);
    inline  void ResetRenderCommandList(SERenderCommandList* renderCommandList, 
        SERenderPassInfo* renderPassInfo);
    inline  void CloseRenderCommandList(SERenderCommandList* renderCommandList);
    inline  void RenderCommandListSetRootSignature(
        SERenderCommandList* renderCommandList, SERootSignature* rootSignature);
    inline  void RenderCommandListSetViewport(SERenderCommandList* renderCommandList, 
        SEViewportState* srcViewport);

    // Root signature stuff.
    inline SERootSignatureHandle* CreateRootSignature(SERootSignature* rootSignature);
    inline void DeleteRootSignature(SERootSignature* rootSignature);

protected:
    GPUDeviceBaseInitialize                           _Initialize;
    GPUDeviceBaseTerminate                            _Terminate;
    GPUDeviceBaseGetMaxAnisFilterLevel				  _GetMaxAnisFilterLevel;
    GPUDeviceBaseSetAnisFilterLevel					  _SetAnisFilterLevel;

    GPUDeviceBaseCreateShader                         _CreateShader;
    GPUDeviceBaseDeleteShader                         _DeleteShader;

    GPUDeviceBaseCreateRenderPassInfo                 _CreateRenderPassInfo;
    GPUDeviceBaseDeleteRenderPassInfo                 _DeleteRenderPassInfo;
    GPUDeviceBaseEnableRenderPassInfo                 _EnableRenderPassInfo;
    GPUDeviceBaseDisableRenderPassInfo                _DisableRenderPassInfo;

    GPUDeviceBaseCreateCommandQueue                   _CreateCommandQueue;
    GPUDeviceBaseDeleteCommandQueue                   _DeleteCommandQueue;

    GPUDeviceBaseCreateCommandAllocator               _CreateCommandAllocator;
    GPUDeviceBaseDeleteCommandAllocator               _DeleteCommandAllocator;

    GPUDeviceBaseCreateCommandList                    _CreateCommandList;
    GPUDeviceBaseDeleteCommandList                    _DeleteCommandList;
    GPUDeviceBaseResetRenderCommandList               _ResetRenderCommandList;
    GPUDeviceBaseCloseRenderCommandList               _CloseRenderCommandList;
    GPUDeviceBaseRenderCommandListSetRootSignature    _RenderCommandListSetRootSignature;
    GPUDeviceBaseRenderCommandListSetViewport         _RenderCommandListSetViewport;

    GPUDeviceBaseCreateRootSignature                  _CreateRootSignature;
    GPUDeviceBaseDeleteRootSignature                  _DeleteRootSignature;

protected:
    SERenderCommandQueue*     mDefaultRenderCommandQueue;
    SERenderCommandAllocator* mDefaultRenderCommandAllocator;
    SERenderCommandList*      mDefaultRenderCommandList;
    SERootSignature*          mDefaultRootSignature;

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