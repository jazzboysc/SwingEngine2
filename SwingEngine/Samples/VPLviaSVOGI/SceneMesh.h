#ifndef RTGI_SceneMesh_H
#define RTGI_SceneMesh_H

#include "SETriangleMesh.h"
#include "SETexture2D.h"
#include "SEStructuredBuffer.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/29/2014
//----------------------------------------------------------------------------
class SceneMesh : public SETriangleMesh
{
public:
	SceneMesh(SEMaterial* material, SERTGICamera* camera);
	virtual ~SceneMesh();

	// Implement base class interface.
	virtual void OnGetShaderConstants();
	virtual void OnUpdateShaderConstants(int technique, int pass);

    SEAxisAlignedBox3f* SceneBB;
	SEVector3f MaterialColor;
    float TessLevel;

private:
    // Scene voxelization pass.
    SEShaderUniform mWorldLocSV;
    SEShaderUniform mSceneBBCenterLocSV;
    SEShaderUniform mSceneBBExtensionLocSV;
    SEShaderUniform mMaterialColorLocSV;
    SEShaderUniform mDimLocSV;
    SEShaderUniform mInv2SceneBBExtensionLocSV;

    // Scene point light shadow map pass.
    SEShaderUniform mWorldLocPointLightShadowMap;
    SEShaderUniform mTessLevelLoc;

    // Scene spot light shadow map pass.
    SEShaderUniform mWorldLocSpotLightShadowMap;

    // G-buffer pass.
    SEShaderUniform mWorldLocGBuffer, mViewLocGBuffer, mProjLocGBuffer;
    SEShaderUniform mWorldCacheLocGBuffer, mViewCacheLocGBuffer, mProjCacheLocGBuffer;
    SEShaderUniform mMaterialColorLocGBuffer;
    SEShaderUniform mUseDiffuseMapLocGBuffer;
    SEShaderUniform mDiffuseMapLocGBuffer;

    // Scene point light RSM pass.
    SEShaderUniform mWorldLocPointLightRSM;
    SEShaderUniform mMaterialColorLocPointLightRSM;
    SEShaderUniform mUseDiffuseMapLocPointLightRSM;
    SEShaderUniform mDiffuseMapLocPointLightRSM;

    // Scene spot light RSM pass.
    SEShaderUniform mWorldLocSpotLightRSM;
    SEShaderUniform mMaterialColorLocSpotLightRSM;
    SEShaderUniform mUseDiffuseMapLocSpotLightRSM;
    SEShaderUniform mDiffuseMapLocSpotLightRSM;
};

typedef SESmartPointer<SceneMesh> SceneMeshPtr;

}

#endif