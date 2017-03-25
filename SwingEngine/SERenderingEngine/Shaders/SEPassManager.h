// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_PassManager_H
#define Swing_PassManager_H

#include "SERenderingEngineLIB.h"
#include "SEReferencable.h"
#include "SEPassBase.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 09/23/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEPassManager : public SEReferencable
{
public:
    SEPassManager();
    ~SEPassManager();

    void AddPass(SEPassBase* pass);
    unsigned int GetPassCount() const;
    SEPassBase* GetPass(unsigned int i) const;

    void CreateDeviceResource(SEGPUDevice* device);

protected:
    std::vector<SEPassBase*> mPasses;
    SEGPUDevice* mGPUDevice;
};

typedef SESmartPointer<SEPassManager> SEPassManagerPtr;

}

#endif