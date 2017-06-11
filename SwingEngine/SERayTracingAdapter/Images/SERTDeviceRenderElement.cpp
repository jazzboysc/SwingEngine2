// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERayTracingAdapterPCH.h"
#include "SERTDeviceRenderElement.h"

using namespace Swing;

//----------------------------------------------------------------------------
SERTDeviceRenderElement::SERTDeviceRenderElement(SERTDeviceRenderElementType type)
    :
    mRenderElementType(type),
    mRenderElementHandle(nullptr)
{
}
//----------------------------------------------------------------------------
SERTDeviceRenderElement::~SERTDeviceRenderElement()
{
    if( mRenderElementHandle )
    {
        // TODO:
        SE_DELETE mRenderElementHandle;
    }
}
//----------------------------------------------------------------------------
void SERTDeviceRenderElement::CreateDeviceResource(SERayTracingDevice&)
{
    if( !mRenderElementHandle )
    {
        // TODO:
    }
}
//----------------------------------------------------------------------------
SERTDeviceRenderElementType SERTDeviceRenderElement::GetRenderElementType() const
{
    return mRenderElementType;
}
//----------------------------------------------------------------------------
SERTDeviceRenderElementHandle* SERTDeviceRenderElement::GetRenderElementHandle()
{
    return mRenderElementHandle;
}
//----------------------------------------------------------------------------