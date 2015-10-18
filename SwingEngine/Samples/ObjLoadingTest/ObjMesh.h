#ifndef RTGI_ObjMesh_H
#define RTGI_ObjMesh_H

#include "SETriangleMesh.h"
#include "SETexture2D.h"

namespace Swing
{

class ObjLoadingTestApp;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 06/14/2015
//----------------------------------------------------------------------------
class ObjMesh : public SETriangleMesh
{
public:
    ObjMesh(SEMaterial* material, SERTGICamera* camera);
    virtual ~ObjMesh();

    // Implement base class interface.
    virtual void OnGetShaderConstants();
    virtual void OnUpdateShaderConstants(int technique, int pass);

	ObjLoadingTestApp* App;

private:
    SEShaderUniform mDiffuseMapLoc;
	SEShaderUniform mShowModeLoc;
};

typedef SESmartPointer<ObjMesh> ObjMeshPtr;

}

#endif