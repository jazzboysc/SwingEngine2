// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_Primitive_H
#define Swing_Primitive_H

#include "SERenderingEngineLIB.h"
#include "SEReferencable.h"
#include "SEVertexBuffer.h"
#include "SEIndexBuffer.h"

namespace Swing
{

enum SEPrimitiveType
{
    PT_Unknown = -1,
    PT_Points,
    PT_Lines,
    PT_Triangles,
    PT_Quads,
    PT_Patches,
    PrimitiveType_Max
};

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 03/09/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEPrimitive : public SEReferencable
{
public:
    SEPrimitive();
    ~SEPrimitive();

    SEVertexBufferPtr VB;
    SEIndexBufferPtr IB;
    SEPrimitiveType Type;
};

typedef SESmartPointer<SEPrimitive> SEPrimitivePtr;

#include "SEPrimitive.inl"

}

#endif