#include "ObjMesh.h"
#include "ObjLoadingTestApp.h"
#include "SEGPUDevice.h"

using namespace Swing;

//----------------------------------------------------------------------------
ObjMesh::ObjMesh(SEMaterial* material, SERTGICamera* camera)
    :
    SETriangleMesh(material, camera)
{
	App = 0;
}
//----------------------------------------------------------------------------
ObjMesh::~ObjMesh()
{
}
//----------------------------------------------------------------------------
void ObjMesh::OnGetShaderConstants()
{
    SETriangleMesh::OnGetShaderConstants();

    SEShaderProgram* program = mMaterial->GetProgram(0, 0);
    program->GetUniformLocation(&mDiffuseMapLoc, "diffuseMap");
	program->GetUniformLocation(&mShowModeLoc, "ShowMode");
}
//----------------------------------------------------------------------------
void ObjMesh::OnUpdateShaderConstants(int technique, int pass)
{
    SETexture2D* diffuseMap = mMaterial->GetDiffuseMap();
    if( diffuseMap )
    {
        SESamplerDesc sd;
        sd.MinFilter = FT_Linear_Linear;
        sd.MagFilter = FT_Linear;
        sd.MipMap = FT_Linear_Linear;
        sd.WrapS = WT_Repeat;
        sd.WrapT = WT_Repeat;

        diffuseMap->BindToSampler(0, &sd);
        mDiffuseMapLoc.SetValue(0);
    }

	if( App )
	{
		mShowModeLoc.SetValue(App->ShowMode);
	}

    // Update pass 1 uniform data.
    if( pass == 0 )
    {
        SETriangleMesh::OnUpdateShaderConstants(technique, pass);
    }
}
//----------------------------------------------------------------------------