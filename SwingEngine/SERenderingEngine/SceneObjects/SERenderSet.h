// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RenderSet_H
#define Swing_RenderSet_H

#include "SERenderingEngineLIB.h"
#include "SEReferencable.h"
#include "SERenderObject.h"

#include <vector>

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/29/2013
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SERenderSet : public SEReferencable
{
public:
    SERenderSet();
    virtual ~SERenderSet();

    void AddRenderObject(SERenderObject* renderObject);
    int GetRenderObjectCount() const;
    SERenderObject* GetRenderObject(int i) const;

    void UpdateRenderCache();

protected:
    std::vector<SERenderObjectPtr> mRenderObjects;
};

typedef SESmartPointer<SERenderSet> SERenderSetPtr;

}

#endif