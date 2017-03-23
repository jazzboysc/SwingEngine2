// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SEAssetUtilityPCH.h"
#include "SEObjLoader.h"
#include "SEObjMetaMaterial.h"

#include <fstream>
#include <sstream>

using namespace Swing;

//----------------------------------------------------------------------------
SEObjLoader::SEObjLoader()
{
}
//----------------------------------------------------------------------------
SEObjLoader::~SEObjLoader()
{
}
//----------------------------------------------------------------------------
bool SEObjLoader::Load(const std::string& filePath, 
	const std::string& fileName, std::vector<SEObjMetaMeshPtr>& meshes)
{
	std::ifstream fileData(filePath + fileName);
    if( !fileData )
    {
        return false;
    }

    std::string curLine;
    std::stringstream lineStream;
    std::string token;
    std::vector<std::string> tokens;
    bool foundNewMesh = false;
    SEObjMetaMesh* newMesh = 0;
	std::string newMeshName;
	std::string newMeshMaterialName;
    int newFaceIndexCount = 0;
	SE_UInt32 globalVertexIndexOffset = 0;
	SE_UInt32 globalVertexListCount = 0;
	SE_UInt32 globalTCoordIndexOffset = 0;
	SE_UInt32 globalTCoordListCount = 0;
    SE_UInt32 globalVertexNormalIndexOffset = 0;
    SE_UInt32 globalVertexNormalListCount = 0;
	SEObjMetaMaterialLib* materialLib = 0;

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
            // Process tokens.

			if( !tokens[0].compare("mtllib") )
			{
				// Process material lib.

				SE_ASSERT(tokens.size() >= 2);
				std::string materialLibFileName = tokens[1];
				materialLib = SE_NEW SEObjMetaMaterialLib();
                materialLib->Load(filePath, materialLibFileName);
			}
            else if( !tokens[0].compare("v") )
            {
                if( !foundNewMesh )
                {
                    newMesh = SE_NEW SEObjMetaMesh();
                    meshes.push_back(newMesh);
                    foundNewMesh = true;

					globalVertexIndexOffset = globalVertexListCount;
					globalTCoordIndexOffset = globalTCoordListCount;
                    globalVertexNormalIndexOffset = globalVertexNormalListCount;
                }

                // Process vertex.
                SE_ASSERT( tokens.size() >= 4 );
                SEVector3f v;
                v.X = (float)atof(tokens[1].c_str());
                v.Y = (float)atof(tokens[2].c_str());
                v.Z = (float)atof(tokens[3].c_str());
                newMesh->AppendVertex(v);

				// Accumulate global vertex list count.
				globalVertexListCount++;
            }
            else if( !tokens[0].compare("vn") )
            {
                // Process vertex normal.
                SE_ASSERT(tokens.size() >= 4);
                SEVector3f vn;
                vn.X = (float)atof(tokens[1].c_str());
                vn.Y = (float)atof(tokens[2].c_str());
                vn.Z = (float)atof(tokens[3].c_str());
                newMesh->AppendVertexNormal(vn);

                // Accumulate global vertex normal list count.
                globalVertexNormalListCount++;
            }
			else if( !tokens[0].compare("vt") )
			{
				// Process texture coordinate.
				SE_ASSERT(tokens.size() >= 4);
				SEVector3f vt;
				vt.X = (float)atof(tokens[1].c_str());
				vt.Y = (float)atof(tokens[2].c_str());
				vt.Z = (float)atof(tokens[3].c_str());
				newMesh->AppendTCoord(vt);

				// Accumulate global texture coordinate list count.
				globalTCoordListCount++;
			}
			else if( !tokens.at(0).compare("g") )
			{
				SE_ASSERT(tokens.size() > 1);
				newMeshName = tokens[1];
			}
			else if( !tokens.at(0).compare("usemtl") )
			{
				SE_ASSERT(tokens.size() > 1);
				newMeshMaterialName = tokens[1];
			}
            else if( !tokens.at(0).compare("f") )
            {
                SE_ASSERT( tokens.size() > 3 );

                if( foundNewMesh )
                {
                    // This is the first time we see current mesh's index data.
                    foundNewMesh = false;

					newMesh->MeshName = newMeshName;
					newMesh->MaterialName = newMeshMaterialName;
                }

				newFaceIndexCount = (int)tokens.size() - 1;

				// TODO:
				// Only support tirangle and quad face for now.
				SE_ASSERT( newFaceIndexCount == 3 || newFaceIndexCount == 4 );

                // Possible layouts:
                // f v       v       v       (v)
                // f v/vt    v/vt    v/vt    (v/vt)
                // f v//vn   v//vn   v//vn   (v//vn)
                // f v/vt/vn v/vt/vn v/vt/vn (v/vt/vn)

				MetaMeshFaceIndex faceIndices;
				for( int i = 1; i <= newFaceIndexCount; ++i )
                {
                    std::stringstream curIndices;
					curIndices.str(tokens[i]);

                    // TODO:
                    // Only interested in vertex index and tCoord for now.
					// obj file's index starts from 1.

					// Get vertex index.
                    std::string vertexIndexToken;
					getline(curIndices, vertexIndexToken, '/');
					SE_UInt32 vertexIndex = (SE_UInt32)atoi(vertexIndexToken.c_str()) - 1;
					vertexIndex -= globalVertexIndexOffset;
					faceIndices.VertexIndices.push_back(vertexIndex);

					// Get tCoord index.
                    if( !curIndices.eof() )
					{
                        std::string tCoordIndexToken;
						getline(curIndices, tCoordIndexToken, '/');

                        if( tCoordIndexToken.size() > 0 )
                        {
                            SE_UInt32 tCoordIndex = (SE_UInt32)atoi(tCoordIndexToken.c_str()) - 1;
                            tCoordIndex -= globalTCoordIndexOffset;
                            faceIndices.TCoordIndices.push_back(tCoordIndex);
                        }
					}

                    // Get vertex normal index.
                    if( !curIndices.eof() )
                    {
                        std::string vertexNormalIndexToken;
                        getline(curIndices, vertexNormalIndexToken, '/');

                        if( vertexNormalIndexToken.size() > 0 )
                        {
                            SE_UInt32 vertexNormalIndex = (SE_UInt32)atoi(vertexNormalIndexToken.c_str()) - 1;
                            vertexNormalIndex -= globalVertexNormalIndexOffset;
                            faceIndices.VertexNormalIndices.push_back(vertexNormalIndex);
                        }
                    }
                }
                newMesh->AppendFaceIndices(faceIndices);
            }
        }
    }

    fileData.close();

	if( materialLib )
	{
		for( int i = 0; i < (int)meshes.size(); ++i )
		{
			meshes[i]->MeshMaterial = materialLib->GetMaterialByName(
				meshes[i]->MaterialName);
		}
	}

    return true;
}
//----------------------------------------------------------------------------