// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SEAssetUtilityPCH.h"
#include "SEObjMetaMaterial.h"

#include <fstream>
#include <sstream>

using namespace Swing;
using namespace std;

//----------------------------------------------------------------------------
SEObjMetaMaterial::SEObjMetaMaterial()
	:
	Ns(0.0f),
	Ni(0.0f),
	Tr(0.0f),
	illum(0)
{
}
//----------------------------------------------------------------------------
SEObjMetaMaterial::~SEObjMetaMaterial()
{
}
//----------------------------------------------------------------------------

typedef unordered_map<string, SEObjMetaMaterialPtr>::iterator 
    SEObjMetaMaterialLibIt;
//----------------------------------------------------------------------------
SEObjMetaMaterialLib::SEObjMetaMaterialLib()
{
}
//----------------------------------------------------------------------------
SEObjMetaMaterialLib::~SEObjMetaMaterialLib()
{
	mMaterials.clear();
}
//----------------------------------------------------------------------------
bool SEObjMetaMaterialLib::Load(const std::string& filePath, 
    const std::string& fileName)
{
    std::ifstream fileData(filePath + fileName);
	if( !fileData )
	{
		return false;
	}

	string curLine;
	stringstream lineStream;
	string token;
	vector<string> tokens;
	SEObjMetaMaterial* newMaterial = 0;

	while( getline(fileData, curLine) )
	{
		lineStream.clear();
		lineStream.str(curLine);
		tokens.clear();

		while( lineStream >> token )
		{
			tokens.push_back(token);
		}

		if( !tokens.empty() )
		{
			if( !tokens[0].compare("newmtl") )
			{
				newMaterial = SE_NEW SEObjMetaMaterial();
				std::string materialName = tokens[1];
				mMaterials[materialName] = newMaterial;
			}
			else if( !tokens[0].compare("Ns") )
			{
				newMaterial->Ns = (float)atof(tokens[1].c_str());
			}
			else if( !tokens[0].compare("Ni") )
			{
				newMaterial->Ni = (float)atof(tokens[1].c_str());
			}
			else if( !tokens[0].compare("Tr") )
			{
				newMaterial->Tr = (float)atof(tokens[1].c_str());
			}
			else if( !tokens[0].compare("illum") )
			{
				newMaterial->illum = (SE_Int32)atoi(tokens[1].c_str());
			}
			else if( !tokens[0].compare("Tf") )
			{
				newMaterial->Tf.X = (float)atof(tokens[1].c_str());
				newMaterial->Tf.Y = (float)atof(tokens[2].c_str());
				newMaterial->Tf.Z = (float)atof(tokens[3].c_str());
			}
			else if( !tokens[0].compare("Ka") )
			{
				newMaterial->Ka.X = (float)atof(tokens[1].c_str());
				newMaterial->Ka.Y = (float)atof(tokens[2].c_str());
				newMaterial->Ka.Z = (float)atof(tokens[3].c_str());
			}
			else if( !tokens[0].compare("Kd") )
			{
				newMaterial->Kd.X = (float)atof(tokens[1].c_str());
				newMaterial->Kd.Y = (float)atof(tokens[2].c_str());
				newMaterial->Kd.Z = (float)atof(tokens[3].c_str());
			}
			else if( !tokens[0].compare("Ks") )
			{
				newMaterial->Ks.X = (float)atof(tokens[1].c_str());
				newMaterial->Ks.Y = (float)atof(tokens[2].c_str());
				newMaterial->Ks.Z = (float)atof(tokens[3].c_str());
			}
			else if( !tokens[0].compare("Ke") )
			{
				newMaterial->Ke.X = (float)atof(tokens[1].c_str());
				newMaterial->Ke.Y = (float)atof(tokens[2].c_str());
				newMaterial->Ke.Z = (float)atof(tokens[3].c_str());
			}
			else if( !tokens[0].compare("map_Ka") )
			{
                newMaterial->map_Ka = filePath + tokens[1];
			}
			else if( !tokens[0].compare("map_Kd") )
			{
                newMaterial->map_Kd = filePath + tokens[1];
			}
			else if( !tokens[0].compare("map_Ks") )
			{
                newMaterial->map_Ks = filePath + tokens[1];
			}
			else if( !tokens[0].compare("map_Ns") )
			{
                newMaterial->map_Ns = filePath + tokens[1];
			}
			else if( !tokens[0].compare("map_d") )
			{
                newMaterial->map_d = filePath + tokens[1];
			}
			else if( !tokens[0].compare("map_bump") )
			{
                newMaterial->map_bump = filePath + tokens[1];
			}
		}
	}

	fileData.close();

	return true;
}
//----------------------------------------------------------------------------
SEObjMetaMaterial* SEObjMetaMaterialLib::GetMaterialByName(
	const std::string& name)
{
	SEObjMetaMaterialLibIt it = mMaterials.find(name);
	if( it != mMaterials.end() )
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------