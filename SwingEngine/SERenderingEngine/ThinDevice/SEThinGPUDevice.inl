// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015
//
// This part of Swing Engine belongs to an abstraction layer of the next 
// generation GPU graphics and compute API. Currently being constructed to 
// support Microsoft's DirectX 12 and Apple's Metal.

//----------------------------------------------------------------------------
SECommandQueueHandle* SEThinGPUDevice::CreateCommandQueue(
    SECommandQueue* commandQueue)
{
    return (this->*_CreateCommandQueue)(commandQueue);
}
//----------------------------------------------------------------------------
void SEThinGPUDevice::DeleteCommandQueue(SECommandQueue* commandQueue)
{
    (this->*_DeleteCommandQueue)(commandQueue);
}
//----------------------------------------------------------------------------
SECommandAllocatorHandle* SEThinGPUDevice::CreateCommandAllocator(
    SECommandAllocator* commandAllocator)
{
    return (this->*_CreateCommandAllocator)(commandAllocator);
}
//----------------------------------------------------------------------------
void SEThinGPUDevice::DeleteCommandAllocator(
    SECommandAllocator* commandAllocator)
{
    (this->*_DeleteCommandAllocator)(commandAllocator);
}
//----------------------------------------------------------------------------