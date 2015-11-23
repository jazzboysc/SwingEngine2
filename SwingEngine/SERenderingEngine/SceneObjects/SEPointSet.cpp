// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SEPointSet.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEPointSet::SEPointSet(SEMaterial* material)
	:
	SERenderObject(material),
	mPointCount(0),
    mComponent(0)
{
    InstanceCount = 1;
    PointSize = 2.0;

    mPrimitive->Type = PT_Points;
}
//----------------------------------------------------------------------------
SEPointSet::~SEPointSet()
{
}
//----------------------------------------------------------------------------
void SEPointSet::Render(int technique, int pass, SESubRenderer*)
{
    // Apply current rendering pass.
    mMaterial->Apply(technique, pass);
}
//----------------------------------------------------------------------------
void SEPointSet::OnRender(SERenderPass*, SEPassInfo* passInfo)
{ 
    // TODO:
    // Hack device for now.
    SEGPUDevice* device = (SEGPUDevice*)passInfo->GetPassInfoHandle()->DeviceBase;
    SE_ASSERT(device);

    device->SetPointSize(PointSize);
    if( InstanceCount > 1 )
    {
        device->DrawPrimitiveInstanced(PT_Points, mPointCount, InstanceCount);
    }
    else
    {
        device->DrawPrimitive(PT_Points, mPointCount);
    }
}
//----------------------------------------------------------------------------
void SEPointSet::CreateDeviceResource(SEGPUDevice* device)
{
    size_t bufferSize = sizeof(float)*mPointCount*mComponent;
    mPrimitive->VB = SE_NEW SEVertexBuffer();
    if( mVertexData.size() > 0 )
    {
        mPrimitive->VB->LoadFromSystemMemory(device, bufferSize,
            (void*)&mVertexData[0], BU_Static_Draw);
    }
    else
    {
        mPrimitive->VB->ReserveImmutableDeviceResource(device, bufferSize);
    }

    // Create shader programs.
    SEGeometryAttributes attributes;
    attributes.Prim = mPrimitive;
    attributes.HasNormal = false;
    attributes.HasTCoord = false;
    attributes.VertexComponentCount = mComponent;
    mMaterial->CreateDeviceResource(device, &attributes);

    // Get shader constants here.
    OnGetShaderConstants();
}
//----------------------------------------------------------------------------
void SEPointSet::LoadFromSystemMemory(unsigned int pointCount, 
    float* vertexData, unsigned int component)
{
	mPointCount = pointCount;
    mComponent = component;
    mVertexData.reserve(mPointCount*component);

    if( vertexData )
    {
        for( unsigned int i = 0; i < mPointCount; ++i )
        {
            for( unsigned int j = 0; j < component; ++j )
            {
                mVertexData.push_back(vertexData[i*component + j]);
            }
        }
    }
}
//----------------------------------------------------------------------------