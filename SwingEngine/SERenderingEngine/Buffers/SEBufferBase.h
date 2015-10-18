// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_BufferBase_H
#define Swing_BufferBase_H

#include "SERenderingEngineLIB.h"
#include "SEGPUDeviceResident.h"
#include "SEGPUResource.h"
#include "SEBufferView.h"

namespace Swing
{

enum SEBufferFormat
{
    BF_Unknown = -1,
    BF_R,
    BF_RG,
    BF_RGB,
    BF_RGBA,
    BF_RGBF,
	BF_RGBAF,
	BF_RGBA16F,
    BF_R32UI,
    BF_R32F,
    BF_Depth,
    BufferFormat_Max
};

enum SEBufferInternalFormat
{
    BIF_Unknown = -1,
    BIF_RGB8,
    BIF_RGBA8,
    BIF_RGB32F,
    BIF_RGBA32F,
    BIF_RGBA32UI,
    BIF_RGB16F,
    BIF_RGBA16F,
    BIF_R32UI,
    BIF_R32F,
    BIF_RG32F,
    BIF_Depth24,
    BufferInternalFormat_Max
};

enum SEBufferComponentType
{
    BCT_Unknown = -1,
    BCT_Unsigned_Byte,
    BCT_Unsigned_Int,
    BCT_Float,
	BCT_Half_Float,
    BufferComponentType_Max
};

enum SERendererDataType
{
    RDT_Unknown = -1,
    RDT_AtomicCounterBuffer,
    RDT_DispatchIndirectBuffer,
    RDT_DrawIndirect,
    RDT_PixelBuffer,
    RDT_StructuredBuffer,
    RDT_TextureBuffer,
    RDT_UniformBuffer,
    RDT_Texture,
    RDT_Image
};

enum SEBindingFlag
{
    BF_Bindless = 0,
    BF_Bind,
    BF_BindIndex,
    BF_BindTo,
    BF_BindIndexTo,
    BindingFlag_Max
};

enum SEBufferUsage
{
    BU_Static_Read = 0,
    BU_Static_Copy,
    BU_Static_Draw,
    BU_Dynamic_Read,
    BU_Dynamic_Copy,
    BU_Dynamic_Draw,
	BU_Stream_Read,
	BU_Stream_Copy,
	BU_Stream_Draw,
    BufferUsage_Max
};

enum SEBufferAccess
{
    BA_Read_Only = 0,
    BA_Write_Only,
    BA_Read_Write,
    BufferAccess_Max
};

enum SEMemoryBarrierType
{
    MBT_Command       = 0x00000001,
    MBT_Structured    = 0x00000002,
    MBT_AtomicCounter = 0x00000004,
    MBT_ALL           = 0x80000000
};

/*
Filter Combination                     | Bilinear | Bilinear | Mipmapping
(MAG_FILTER / MIN_FILTER)              | (Near)   | (FAR)    |
---------------------------------------+----------+----------+------------
FT_Nearest / FT_Nearest_Nearest        | Off      | Off      | Standard
FT_Nearest / FT_Linear_Nearest         | Off      | On       | Standard
FT_Nearest / FT_Nearest_Linear         | Off      | Off      | Trilinear filtering
FT_Nearest / FT_Linear_Linear          | Off      | On       | Trilinear filtering
FT_Nearest / FT_Nearest                | Off      | Off      | None
FT_Nearest / FT_Linear                 | Off      | On       | None
FT_Linear / FT_Nearest_Nearest         | On       | Off      | Standard
FT_Linear / FT_Linear_Nearest          | On       | On       | Standard
FT_Linear / FT_Nearest_Linear          | On       | Off      | Trilinear filtering
FT_Linear / FT_Linear_Linear           | On       | On       | Trilinear filtering
FT_Linear / FT_Nearest                 | On       | Off      | None
FT_Linear / FT_Linear                  | On       | On       | None
*/

enum SEFilterType
{
    FT_Nearest = 0,
    FT_Linear,
    FT_Nearest_Nearest,
    FT_Linear_Nearest,
    FT_Nearest_Linear,
    FT_Linear_Linear,
    FilterType_Max
};

enum SEWrapType
{
    WT_Clamp = 0,
    WT_Repeat,
    WrapType_Max
};

struct SE_RENDERING_ENGINE_API SESamplerDesc
{
    SEFilterType MinFilter;
    SEFilterType MagFilter;
    SEFilterType MipMap;
    SEWrapType WrapS;
    SEWrapType WrapT;
    SEWrapType WrapR;
};

struct SE_RENDERING_ENGINE_API SERendererInputDataView
{
    SERendererDataType Type;
    SEBindingFlag BindingType;
    int BindingSlot;
    SESamplerDesc Sampler;
};

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/29/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEBufferBase : public SEGPUDeviceResident
{
public:
    SEBufferBase();
    virtual ~SEBufferBase();

};

typedef SESmartPointer<SEBufferBase> SEBufferBasePtr;

}

#endif