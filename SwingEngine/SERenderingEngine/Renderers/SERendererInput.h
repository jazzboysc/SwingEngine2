// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_RendererInput_H
#define Swing_RendererInput_H

#include "SERenderingEngineLIB.h"
#include "SEReferencable.h"
#include "SEIndexableBuffer.h"
#include <string>

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/30/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SERendererInput : public SEReferencable
{
public:
    SERendererInput(const std::string& name, SEBufferBase* inputBuffer, 
        SERendererInputDataView* view);
    virtual ~SERendererInput();

    void Enable();
    void Disable();

    std::string Name;
    SEBufferBasePtr InputBuffer;
    SEBufferViewPtr InputBufferView;
    SERendererInputDataView View;
};

typedef SESmartPointer<SERendererInput> SERendererInputPtr;

}

#endif