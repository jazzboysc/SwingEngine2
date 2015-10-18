// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_GBufferRenderer_H
#define Swing_GBufferRenderer_H

#include "SERenderingEngineLIB.h"
#include "SESubRenderer.h"

namespace Swing
{

#define RTGI_GBuffer_Position_Name "Position"
#define RTGI_GBuffer_Normal_Name   "Normal"
#define RTGI_GBuffer_Albedo_Name   "Albedo"
#define RTGI_GBuffer_RPC_Name      "RPC"

struct SE_RENDERING_ENGINE_API SEGBufferDesc
{
    int Width;
    int Height;

    SEBufferFormat PositionFormat;
    SEBufferFormat NormalFormat;
    SEBufferFormat AlbedoFormat;
    SEBufferFormat RPCFormat;

    bool PositionMipmap;
    bool NormalMipmap;
    bool AlbedoMipmap;
    bool RPCMipmap;

    SEGBufferDesc()
    {
        Width = 0;
        Height = 0;
        PositionFormat = BF_Unknown;
        NormalFormat = BF_Unknown;
        AlbedoFormat = BF_Unknown;
        RPCFormat = BF_Unknown;
        PositionMipmap = false;
        NormalMipmap = false;
        AlbedoMipmap = false;
        RPCMipmap = false;
    }
};

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/29/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEGBufferRenderer : public SESubRenderer
{
public:
    SEGBufferRenderer(SEGPUDevice* device, SERenderSet* renderSet = 0);
    virtual ~SEGBufferRenderer();

    void Initialize(SEGBufferDesc* desc);
    void Render(int technique, int pass, SERTGICamera* camera);
    const SEGBufferDesc& GetGBufferDesc() const;

protected:
    SEPipelineStateBlockPtr mPSB;
    SEGBufferDesc mGBufferDesc;
};

typedef SESmartPointer<SEGBufferRenderer> SEGBufferRendererPtr;

}

#endif