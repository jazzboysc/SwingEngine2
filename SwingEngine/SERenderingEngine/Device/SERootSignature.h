// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017
//
// This part of Swing Engine belongs to an abstraction layer of the next 
// generation GPU graphics and compute API. Currently being constructed to 
// support Microsoft's DirectX 12 and Apple's Metal.

#ifndef Swing_RootSignature_H
#define Swing_RootSignature_H

#include "SERenderingEngineLIB.h"
#include "SEGPUDeviceResident.h"
#include "SEGPUDeviceBaseChild.h"

namespace Swing
{

enum SERootSignatureFlag
{
    RSF_Unknown                            = 0,
    RSF_Allow_Input_Assembler_Input_Layout = 1,
    RSF_Deny_VS_Root_Access                = 2,
    RSF_Deny_TCS_Root_Access               = 4,
    RSF_Deny_TES_Root_Access               = 8,
    RSF_Deny_GS_Root_Access                = 16,
    RSF_Deny_FS_Root_Access                = 32,
    RSF_Allow_Stream_Output                = 64
};

struct SE_RENDERING_ENGINE_API SERootSignatureInfo
{
    unsigned int Flags;

    SERootSignatureInfo()
        :
        Flags(RSF_Unknown)
    {
    }
};

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/22/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SERootSignature : public SEGPUDeviceResident
{
public:
    SERootSignature(const SERootSignatureInfo& rootSignatureInfo);
    virtual ~SERootSignature();

    void CreateDeviceChild(SEGPUDeviceBase* device);

    SERootSignatureHandle* GetRootSignatureHandle() const;
    const SERootSignatureInfo& GetRootSignatureInfo() const;

protected:
    SERootSignatureHandle* mRootSignatureHandle;
    SERootSignatureInfo mRootSignatureInfo;
};

typedef SESmartPointer<SERootSignature> SERootSignaturePtr;

}

#endif