// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SERendererInput.h"
#include "SETexture.h"

using namespace Swing;

//----------------------------------------------------------------------------
void EnableTexture(SERendererInput* input)
{
    SETexture* texture = (SETexture*)(SEBufferBase*)input->InputBuffer;
    texture->BindToSampler(input->View.BindingSlot, &input->View.Sampler);
}
//----------------------------------------------------------------------------
void EnableBuffer(SERendererInput* input)
{
    SEBuffer* buffer = (SEBuffer*)(SEBufferBase*)input->InputBuffer;
    switch( input->View.BindingType )
    {
    case BF_Bind:
        buffer->Bind();
        break;

    case BF_BindIndex:
        ((SEIndexableBuffer*)buffer)->Bind(input->View.BindingSlot);
        break;

    case BF_BindTo:
        buffer->BindTo(input->InputBufferView);
        break;

    default:
        SE_ASSERT(false);
        break;
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
SERendererInput::SERendererInput(const std::string& name,
    SEBufferBase* inputBuffer, SERendererInputDataView* view)
{
    Name = name;
    InputBuffer = inputBuffer;
    View = *view;

    SE_ASSERT(View.Type != RDT_Unknown);
    if( View.Type != RDT_Texture && View.Type != RDT_Image )
    {
        // Create a buffer view's system footage.
        SEBufferViewDesc viewDesc;
        viewDesc.Type = (SEBufferType)View.Type;
        InputBufferView = SE_NEW SEBufferView(viewDesc);
    }
}
//----------------------------------------------------------------------------
SERendererInput::~SERendererInput()
{
    InputBuffer = 0;
    InputBufferView = 0;
}
//----------------------------------------------------------------------------
void SERendererInput::Enable()
{
    switch( View.Type )
    {
    case RDT_Texture:
        EnableTexture(this);
        break;

    case RDT_StructuredBuffer:
    case RDT_AtomicCounterBuffer:
    case RDT_UniformBuffer:
    case RDT_DispatchIndirectBuffer:
    case RDT_PixelBuffer:
    case RDT_TextureBuffer:
        EnableBuffer(this);
        break;

    default:
        SE_ASSERT(false);
        break;
    }
}
//----------------------------------------------------------------------------
void SERendererInput::Disable()
{
    // Nothing to do.
}
//----------------------------------------------------------------------------