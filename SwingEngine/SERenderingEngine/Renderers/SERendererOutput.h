// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_RendererOutput_H
#define Swing_RendererOutput_H

#include "SERenderingEngineLIB.h"
#include "SEReferencable.h"
#include "SEIndexableBuffer.h"
#include <string>

namespace Swing
{

enum SERendererOutputType
{
    ROT_Buffer = 0,
    ROT_Texture
};

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/29/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SERendererOutput : public SEReferencable
{
public:
    SERendererOutput(const std::string& name, SEBufferBase* outputBuffer, 
        SERendererOutputType outputType = ROT_Texture, 
        SEBindingFlag flag = BF_Bindless, SEBufferType viewType = BT_Typeless,
        unsigned int binding = 0, bool reset = false, int resetValue = 0);
    virtual ~SERendererOutput();

    void Enable();
    void Disable();

    std::string Name;
    SEBufferBasePtr OutputBuffer;
    SEBufferViewPtr OutputBufferView;
    SERendererOutputType OutputType;
    SEBindingFlag Flag;
    unsigned int Binding;
    bool Reset;
    int ResetValue;

private:
    SERendererOutput();
};

typedef SESmartPointer<SERendererOutput> SERendererOutputPtr;

}

#endif