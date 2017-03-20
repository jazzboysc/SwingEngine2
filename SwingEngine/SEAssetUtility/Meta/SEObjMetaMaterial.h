// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_ObjMetaMaterial_H
#define Swing_ObjMetaMaterial_H

#include "SEAssetUtilityLIB.h"
#include "SEReferencable.h"
#include "SEVector3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 07/02/2015
//----------------------------------------------------------------------------
class SE_ASSET_UTILITY_API SEObjMetaMaterial : public SEReferencable
{
public:
	SEObjMetaMaterial();
	~SEObjMetaMaterial();

	float Ns;         // Specular highlight component
	float Ni;
	float Tr;         // Transparency
	SE_Int32 illum;   // illumination model
	SEVector3f Tf;
	SEVector3f Ka, Kd, Ks, Ke;  // Ambient, diffuse, specular colors

	std::string map_Ka;    // Ambient texture map
	std::string map_Kd;    // Diffuse texture map
	std::string map_Ks;    // Specular texture map
	std::string map_Ns;    // Specular highlight component map
	std::string map_d;     // Alpha texture map
	std::string map_bump;  // Bump map
};

typedef SESmartPointer<SEObjMetaMaterial> SEObjMetaMaterialPtr;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 07/02/2015
//----------------------------------------------------------------------------
class SE_ASSET_UTILITY_API SEObjMetaMaterialLib : public SEReferencable
{
public:
	SEObjMetaMaterialLib();
	~SEObjMetaMaterialLib();

	// Load data from a ".mtl" file and create corresponding meta materials.
    bool Load(const std::string& filePath, const std::string& fileName);

	SEObjMetaMaterial* GetMaterialByName(const std::string& name);

private:
	std::unordered_map<std::string, SEObjMetaMaterialPtr> mMaterials;
};

typedef SESmartPointer<SEObjMetaMaterialLib> SEObjMetaMaterialLibPtr;

}

#endif