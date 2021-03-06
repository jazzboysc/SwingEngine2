// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERenderingEnginePCH.h"
#include "SEPassManager.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEPassManager::SEPassManager()
    :
    mGPUDevice(0)
{
}
//----------------------------------------------------------------------------
SEPassManager::~SEPassManager()
{
    for( int i = 0; i < (int)mPasses.size(); ++i )
    {
        SE_DELETE mPasses[i];
    }
}
//----------------------------------------------------------------------------
void SEPassManager::AddPass(SEPassBase* pass)
{
    SE_ASSERT(pass);
    mPasses.push_back(pass);
}
//----------------------------------------------------------------------------
unsigned int SEPassManager::GetPassCount() const
{
    return (unsigned int)mPasses.size();
}
//----------------------------------------------------------------------------
SEPassBase* SEPassManager::GetPass(unsigned int i) const
{
    return mPasses[i];
}
//----------------------------------------------------------------------------
void SEPassManager::CreateDeviceResource(SEGPUDevice* device)
{
    mGPUDevice = device;

    for( int i = 0; i < (int)mPasses.size(); ++i )
    {
        mPasses[i]->CreateDeviceResource(device);
    }
}
//----------------------------------------------------------------------------