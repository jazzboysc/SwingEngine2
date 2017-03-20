// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RenderObject_H
#define Swing_RenderObject_H

#include "SERenderingEngineLIB.h"
#include "SERenderCache.h"
#include "SEMaterial.h"
#include "SERTGICamera.h"
#include "SESpatialInfo.h"
#include "SEPrimitive.h"
#include "SERenderPass.h"
#include "SEShaderUniform.h"
#include "SEMaterial.h"
#include <cstring>

namespace Swing
{

class SESubRenderer;
class SEVoxelizer;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 09/14/2013
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SERenderObject : public SEReferencable
{
public:
	SERenderObject(SEMaterial* material);
	virtual ~SERenderObject();

    virtual void SetCamera(SERTGICamera* camera);
    virtual SERTGICamera* GetCamera() const;

    virtual int GetVoxelizerRasterDimension(SEVoxelizer* voxelizer);

    // SEPrimitive access.
    virtual SEPrimitive* GetPrimitive() const;

    // Material access.
    virtual SEMaterial* GetMaterial() const;

    // Spatial information delegate functions.
    inline void SetSpatialInfo(SESpatialInfo* spatialInfo);
    inline SESpatialInfo* GetSpatialInfo();
    virtual void SetWorldTransform(const SEMatrix4f& worldTrans);
    virtual SEMatrix4f GetWorldTransform() const;
    virtual void SetWorldTranslation(const SEVector3f& translation);
    virtual SEVector3f GetWorldTranslation() const;
    virtual void SetWorldScale(const SEVector3f& scale);
    virtual SEVector3f GetWorldScale() const;

    // Render cache delegate functions.
    virtual void SetRenderCache(SERenderCache* renderCache);
    virtual SERenderCache* GetRenderCache();
    virtual void UpdateRenderCache();

	virtual void Render(int technique, int pass, 
        SESubRenderer* subRenderer = 0) = 0;
	virtual void OnUpdateShaderConstants(int technique, int pass) = 0;
    virtual void OnRender(SERenderPass*, SERenderPassInfo*){};
	virtual void OnEnableBuffers(){};
	virtual void OnDisableBuffers(){};

	std::string Name;

protected:
    SEPrimitivePtr mPrimitive;
	SEMaterialPtr mMaterial;
    SERTGICamera* mCamera;
    SESpatialInfoPtr mSpatialInfo;
    SERenderCachePtr mRenderCache;
};

typedef SESmartPointer<SERenderObject> SERenderObjectPtr;

#include "SERenderObject.inl"

}

#endif