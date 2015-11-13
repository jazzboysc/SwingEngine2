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
typedef SEPassInfoHandle* (SEGPUDeviceBase::*GPUDeviceBaseCreatePassInfo)(
    SEPassInfo* passInfo, SEShaderProgram* program, SEGeometryAttributes* geometryAttr, 
    SEPipelineStateBlock* psb);
typedef void (SEGPUDeviceBase::*GPUDeviceBaseDeletePassInfo)(SEPassInfo* passInfo);
typedef void (SEGPUDeviceBase::*GPUDeviceBaseEnablePassInfo)(SEPassInfo* passInfo);
typedef void (SEGPUDeviceBase::*GPUDeviceBaseDisablePassInfo)(SEPassInfo* passInfo);

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

    // Pass info stuff.
    inline 	SEPassInfoHandle* CreatePassInfo(SEPassInfo* passInfo,
        SEShaderProgram* program, SEGeometryAttributes* geometryAttr,
        SEPipelineStateBlock* psb);
    inline 	void DeletePassInfo(SEPassInfo* passInfo);
    inline void EnablePassInfo(SEPassInfo* passInfo);
    inline void DisablePassInfo(SEPassInfo* passInfo);

protected:
    GPUDeviceBaseInitialize                           _Initialize;
    GPUDeviceBaseTerminate                            _Terminate;
    GPUDeviceBaseGetMaxAnisFilterLevel				  _GetMaxAnisFilterLevel;
    GPUDeviceBaseSetAnisFilterLevel					  _SetAnisFilterLevel;
    GPUDeviceBaseCreateShader                         _CreateShader;
    GPUDeviceBaseDeleteShader                         _DeleteShader;
    GPUDeviceBaseCreatePassInfo                       _CreatePassInfo;
    GPUDeviceBaseDeletePassInfo                       _DeletePassInfo;
    GPUDeviceBaseEnablePassInfo                       _EnablePassInfo;
    GPUDeviceBaseDisablePassInfo                      _DisablePassInfo;

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