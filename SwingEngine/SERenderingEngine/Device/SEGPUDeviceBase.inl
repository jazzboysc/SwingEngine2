// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

//----------------------------------------------------------------------------
void SEGPUDeviceBase::Terminate()
{
    (this->*_Terminate)();
}
//----------------------------------------------------------------------------
void SEGPUDeviceBase::Initialize(SEGPUDeviceDescription* deviceDesc)
{
    mDeviceDesc = *deviceDesc;
    (this->*_Initialize)(deviceDesc);
}
//----------------------------------------------------------------------------
void SEGPUDeviceBase::SetAnisFilterLevel(int maxAnisFilterLevel)
{
    (this->*_SetAnisFilterLevel)(maxAnisFilterLevel);
}
//----------------------------------------------------------------------------
void SEGPUDeviceBase::GetMaxAnisFilterLevel(int* maxAnisFilterLevel)
{
    (this->*_GetMaxAnisFilterLevel)(maxAnisFilterLevel);
}
//----------------------------------------------------------------------------
SEShaderHandle* SEGPUDeviceBase::CreateShader(SEShader* shader)
{
    return (this->*_CreateShader)(shader);
}
//----------------------------------------------------------------------------
void SEGPUDeviceBase::DeleteShader(SEShader* shader)
{
    (this->*_DeleteShader)(shader);
}
//----------------------------------------------------------------------------
SECommandQueueHandle* SEGPUDeviceBase::CreateCommandQueue(
    SECommandQueue* commandQueue)
{
    return (this->*_CreateCommandQueue)(commandQueue);
}
//----------------------------------------------------------------------------
void SEGPUDeviceBase::DeleteCommandQueue(SECommandQueue* commandQueue)
{
    (this->*_DeleteCommandQueue)(commandQueue);
}
//----------------------------------------------------------------------------
SECommandAllocatorHandle* SEGPUDeviceBase::CreateCommandAllocator(
    SECommandAllocator* commandAllocator, SECommandList* commandList)
{
    return (this->*_CreateCommandAllocator)(commandAllocator, commandList);
}
//----------------------------------------------------------------------------
void SEGPUDeviceBase::DeleteCommandAllocator(
    SECommandAllocator* commandAllocator, SECommandList* commandList)
{
    (this->*_DeleteCommandAllocator)(commandAllocator, commandList);
}
//----------------------------------------------------------------------------
SECommandListHandle* SEGPUDeviceBase::CreateCommandList(
    SECommandList* commandList, SECommandAllocator* commandAllocator)
{
    return (this->*_CreateCommandList)(commandList, commandAllocator);
}
//----------------------------------------------------------------------------
void SEGPUDeviceBase::DeleteCommandList(SECommandList* commandList,
    SECommandAllocator* commandAllocator)
{
    (this->*_DeleteCommandList)(commandList, commandAllocator);
}
//----------------------------------------------------------------------------