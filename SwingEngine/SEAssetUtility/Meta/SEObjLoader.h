// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_ObjLoader_H
#define Swing_ObjLoader_H

#include "SEAssetUtilityLIB.h"
#include "SEReferencable.h"
#include "SEObjMetaMesh.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 07/01/2015
//----------------------------------------------------------------------------
class SE_ASSET_UTILITY_API SEObjLoader : public SEReferencable
{
public:
    SEObjLoader();
    ~SEObjLoader();

    // Load data from a ".obj" file and create corresponding meta meshes.
	bool Load(const std::string& filePath, const std::string& fileName, 
		std::vector<SEObjMetaMeshPtr>& meshes);
};

typedef SESmartPointer<SEObjLoader> SEObjLoaderPtr;

}

#endif