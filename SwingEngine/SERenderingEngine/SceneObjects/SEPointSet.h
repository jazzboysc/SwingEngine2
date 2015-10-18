// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_PointSet_H
#define Swing_PointSet_H

#include "SERenderingEngineLIB.h"
#include "SERenderObject.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 09/19/2013
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEPointSet : public SERenderObject
{
public:
	SEPointSet(SEMaterial* material);
	~SEPointSet();

	// Implement base class interface.
	void Render(int technique, int pass, SESubRenderer* subRenderer = 0) override;
    virtual void OnRender(SEPass* pass, SEPassInfo* passInfo);

	// Load data from user specified memory location.
    void LoadFromSystemMemory(unsigned int pointCount, float* vertexData,
        unsigned int component);

    void CreateDeviceResource(SEGPUDevice* device);
	void OnUpdateShaderConstants(int, int) override {};
	virtual void OnGetShaderConstants() {};

    unsigned int InstanceCount;
    float PointSize;

protected:
	unsigned int mPointCount;
    unsigned int mComponent;
	std::vector<float> mVertexData;
};

typedef SESmartPointer<SEPointSet> SEPointSetPtr;

}

#endif