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
        mRenderElementHandle->RTDevice->DeleteRenderElement(this);
        SE_DELETE mRenderElementHandle;
        mRenderElementHandle = nullptr;
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
void SERTDeviceRenderElement::SetRenderElementHandle(SERTDeviceRenderElementHandle* handle)
{
    SE_ASSERT( mRenderElementHandle == nullptr );
    if( !mRenderElementHandle )
    {
        mRenderElementHandle = handle;
    }
}
//----------------------------------------------------------------------------
void SERTDeviceRenderElement::SaveRenderElementToFile(const std::string& fileName, SERTDeviceImageFileType fileType)
{
    if( mRenderElementHandle )
    {
        mRenderElementHandle->RTDevice->SaveRenderElementToFile(this, fileName, fileType);
    }
}
//----------------------------------------------------------------------------