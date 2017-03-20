// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_IndexableBuffer_H
#define Swing_IndexableBuffer_H

#include "SERenderingEngineLIB.h"
#include "SEBuffer.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 01/30/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEIndexableBuffer : public SEBuffer
{
public:
    SEIndexableBuffer(SEBufferType type);
    ~SEIndexableBuffer();

    using SEBuffer::Bind;
    void Bind(unsigned int index);

    using SEBuffer::BindTo;
    void BindTo(unsigned int index, SEBufferView* view);

    void UpdateSubData(unsigned int index, int offset, size_t size,
        void* data);

private:
    SEIndexableBuffer();
};

typedef SESmartPointer<SEIndexableBuffer> SEIndexableBufferBufferPtr;

}


#endif