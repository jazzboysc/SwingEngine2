// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_SubRenderer_H
#define Swing_SubRenderer_H

#include "SERenderingEngineLIB.h"
#include "SEReferencable.h"
#include "SEGPUDevice.h"
#include "SETexture.h"
#include "SERendererInput.h"
#include "SERendererOutput.h"
#include "SERenderSet.h"
#include "SERTGICamera.h"
#include "SEPipelineStateBlock.h"
#include "SEGPUTimer.h"
#include "SEFrameBuffer.h"
#include "SEBuffer.h"

#include <vector>

namespace Swing
{

enum SESubRendererOutput
{
    SRO_Unknown       = 0,
    SRO_FrameBuffer   = 1,
    SRO_GenericBuffer = 2,
    SRO_GenericImage  = 4,
    SRO_BackBuffer    = 8
};

typedef SEBufferBase* (*SubRendererCreateRendererData)(SEGPUDevice* device, 
    int size, SEBufferUsage usage);

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/29/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SESubRenderer : public SEReferencable
{
public:
    SESubRenderer(SEGPUDevice* device, SERenderSet* renderSet = 0);
    virtual ~SESubRenderer();

    // Scene rendered by this renderer.
    void SetRenderSet(SERenderSet* renderSet);
    SERenderSet* GetRenderSet() const;

    // Renderer Inputs stuff.
    enum { MAX_INPUT_DEPENDENCY_COUNT = 16 };
    void AddInputDependency(SESubRenderer* producer, const std::string& srcName,
        SERendererInputDataView* view);
    SERendererInput* GetInputDependency(int i) const;
    void ClearInputDependency();

    // SEFrameBuffer stuff.
    void AddFrameBufferTarget(const std::string& name, int width, int height, 
        int depth, SETextureType type, SEBufferFormat format, 
        bool generateMipmap = false);
    int GetFrameBufferTargetCount() const;
    SERendererOutput* GetFrameBufferTarget(int i) const;
    SERendererOutput* GetFrameBufferTargetByName(const std::string& name) const;
    SERendererOutput* GetDepthTarget() const;
    SETexture* GetFrameBufferTexture(int i) const;
    SETexture* GetFrameBufferTextureByName(const std::string& name) const;
    SETexture* GetDepthTexture() const;

    void CreateFrameBuffer(int depthWidth, int depthHeight, int depthCount,
        SETextureType depthType);

    // GenericBuffer stuff.
    void AddGenericBufferTarget(const std::string& name, 
        SERendererDataType bufferType, int size, SEBufferUsage usage, 
        SEBindingFlag flag, unsigned int binding, bool reset = false, 
        int resetValue = 0);
    int GetGenericBufferTargetCount() const;
    SERendererOutput* GetGenericBufferTarget(int i) const;
    SERendererOutput* GetGenericBufferTargetByName(const std::string& name) const;
    SEBuffer* GetGenericBuffer(int i) const;
    SEBuffer* GetGenericBufferByName(const std::string& name) const;

    // Rendering stuff.
    virtual void Render(int technique, int pass, unsigned int outputFlag, 
        SEPipelineStateBlock* psb, SERTGICamera* camera = 0);
    virtual void RenderSingle(SERenderObject* object, int technique, int pass, 
        SESubRendererOutput outputFlag, SEPipelineStateBlock* psb, 
        SERTGICamera* camera = 0);
    virtual void OnRender(int technique, int pass, SERTGICamera* camera);

    // Timer stuff.
    void SetTimer(SEGPUTimer* timer);
    SEGPUTimer* GetTimer() const;
    double GetTimeElapsed() const;

protected:
    void PreRender(unsigned int outputFlag, SEPipelineStateBlock* psb);
    void PostRender(unsigned int outputFlag, SEPipelineStateBlock* psb);

    SEGPUDevice* mDevice;

    // InOuts.


    // SEBuffer inputs.
    std::vector<SERendererInputPtr> mInputs;

    // SEBuffer outputs.
    std::vector<SERendererOutputPtr> mFrameBufferTargets;
    std::vector<SETexture*> mFrameBufferTargetsMipmapped;
    SERendererOutputPtr mDepthTarget;
    SEFrameBufferPtr mFrameBuffer;
    std::vector<SERendererOutputPtr> mGenericBufferTargets;

    // Scene Inputs.
    SERenderSetPtr mRenderSet;

    SEGPUTimerPtr mTimer;

private:
    SESubRenderer();
    static SubRendererCreateRendererData msFactoryFunctions[7];
};

typedef SESmartPointer<SESubRenderer> SESubRendererPtr;

}

#endif