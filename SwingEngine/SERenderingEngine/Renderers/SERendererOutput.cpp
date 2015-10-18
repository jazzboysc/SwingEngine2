// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SERendererOutput.h"

using namespace Swing;

//----------------------------------------------------------------------------
SERendererOutput::SERendererOutput()
{
}
//----------------------------------------------------------------------------
SERendererOutput::SERendererOutput(const std::string& name, 
    SEBufferBase* outputBuffer, SERendererOutputType outputType, SEBindingFlag flag,
    SEBufferType viewType, unsigned int binding, bool reset, int resetValue)
{
    Name = name;
    OutputBuffer = outputBuffer;
    OutputType = outputType;
    Flag = flag;
    Binding = binding;
    Reset = reset;
    ResetValue = resetValue;

    if( Flag == BF_BindTo || Flag == BF_BindIndexTo )
    {
        SEBufferViewDesc viewDesc;
        viewDesc.Type = viewType;
        OutputBufferView = SE_NEW SEBufferView(viewDesc);
    }
}
//----------------------------------------------------------------------------
SERendererOutput::~SERendererOutput()
{
    OutputBuffer = 0;
    OutputBufferView = 0;
}
//----------------------------------------------------------------------------
void SERendererOutput::Enable()
{
    SE_ASSERT(OutputType == ROT_Buffer);
    SEBuffer* buffer = (SEBuffer*)(SEBufferBase*)OutputBuffer;
    switch( Flag )
    {
    case BF_Bind:
        buffer->Bind();
        break;

    case BF_BindIndex:
        ((SEIndexableBuffer*)buffer)->Bind(Binding);
        break;

    case BF_BindTo:
        buffer->BindTo(OutputBufferView);
        break;

    case BF_BindIndexTo:
        ((SEIndexableBuffer*)buffer)->Bind(Binding);
        buffer->BindTo(OutputBufferView);
        break;

    default:
        SE_ASSERT(false);
        break;
    }

    if( Reset )
    {
        size_t bufferSize = buffer->GetSize();
        int* bufferData = (int*)buffer->Map(BA_Write_Only);
        SE_ASSERT(bufferData);
        memset(bufferData, ResetValue, bufferSize);
        buffer->Unmap();
    }
}
//----------------------------------------------------------------------------
void SERendererOutput::Disable()
{
    // Nothing to do.
}
//----------------------------------------------------------------------------