// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SEAssetUtilityPCH.h"
#include "SEObjMetaMesh.h"

using namespace Swing;
using namespace std;

//----------------------------------------------------------------------------
class VertexAttr
{
public:
	VertexAttr();

	bool operator == (const VertexAttr& attr) const;
	bool operator < (const VertexAttr& attr) const;

	SE_Int32 V, T, N;
};
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
VertexAttr::VertexAttr()
	:
	V(-1),
	T(-1),
    N(-1)
{
}
//----------------------------------------------------------------------------
bool VertexAttr::operator==(const VertexAttr& attr) const
{
	return V == attr.V && T == attr.T && N == attr.N;
}
//----------------------------------------------------------------------------
bool VertexAttr::operator<(const VertexAttr& attr) const
{
	if( V < attr.V )
	{
		return true;
	}

	if( V > attr.V )
	{
		return false;
	}

    if( T < attr.T )
    {
        return true;
    }

    if( T > attr.T )
    {
        return false;
    }

    return N < attr.N;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
SEObjMetaMesh::SEObjMetaMesh()
    :
    mVertexCount(0),
	mTCoordCount(0),
    mVertexNormalCount(0),
    mFaceCount(0),
    mFaceFlag(0),
	mUnified(false)
{
}
//----------------------------------------------------------------------------
SEObjMetaMesh::~SEObjMetaMesh()
{
}
//----------------------------------------------------------------------------
std::vector<SEVector3f>& SEObjMetaMesh::GetVertexData()
{
    return mVertexData;
}
//----------------------------------------------------------------------------
std::vector<SEVector3f>& SEObjMetaMesh::GetTCoordData(unsigned int channel)
{
    SE_ASSERT(channel == 0);
    return mTCoordData;
}
//----------------------------------------------------------------------------
std::vector<SEVector3f>& SEObjMetaMesh::GetVertexNormalData()
{
    return mVertexNormalData;
}
//----------------------------------------------------------------------------
std::vector<SEVector3f>& SEObjMetaMesh::GetFaceNormalData()
{
    return mFaceNormalData;
}
//----------------------------------------------------------------------------
std::vector<MetaMeshFaceIndex>& SEObjMetaMesh::GetIndexData()
{
    return mIndexData;
}
//----------------------------------------------------------------------------
bool SEObjMetaMesh::HasTCoord(unsigned int channel) const
{
    if( channel == 0 )
    {
        return mTCoordCount > 0;
    }
    
    return false;
}
//----------------------------------------------------------------------------
bool SEObjMetaMesh::HasNormal() const
{
    return mVertexNormalCount > 0;
}
//----------------------------------------------------------------------------
unsigned int SEObjMetaMesh::GetTCoordChannelCount() const
{
    return mTCoordCount > 0 ? 1 : 0;
}
//----------------------------------------------------------------------------
void SEObjMetaMesh::AppendVertex(SEVector3f& vertex)
{
    mVertexData.push_back(vertex);
    mVertexCount++;
}
//----------------------------------------------------------------------------
void SEObjMetaMesh::AppendTCoord(SEVector3f& tCoord)
{
	mTCoordData.push_back(tCoord);
	mTCoordCount++;
}
//----------------------------------------------------------------------------
void SEObjMetaMesh::AppendVertexNormal(SEVector3f& vertexNormal)
{
    mVertexNormalData.push_back(vertexNormal);
    mVertexNormalCount++;
}
//----------------------------------------------------------------------------
void SEObjMetaMesh::AppendFaceIndices(MetaMeshFaceIndex& indices)
{
	SE_UInt32 flag = (SE_UInt32)indices.VertexIndices.size() - 2;
	mFaceFlag |= flag;
	mIndexData.push_back(indices);
    mFaceCount++;
}
//----------------------------------------------------------------------------
void SEObjMetaMesh::ConvertToTriangleMesh()
{
	SE_ASSERT( mFaceFlag != (SE_UInt32)MetaMeshFaceType::MMFT_Unknown );
	if( mFaceFlag == (SE_UInt32)MetaMeshFaceType::MMFT_TriangleIndex )
	{
		return;
	}

	if( mFaceCount > 0 )
	{
		std::vector<MetaMeshFaceIndex> newIndexData;
		newIndexData.reserve(mFaceCount);

		for( int i = 0; i < mFaceCount; ++i )
		{
			if( (int)mIndexData[i].VertexIndices.size() == 3 )
			{
				newIndexData.push_back(mIndexData[i]);
			}
			else if( (int)mIndexData[i].VertexIndices.size() == 4 )
			{
				SE_UInt32 vi0 = mIndexData[i].VertexIndices[0];
				SE_UInt32 vi1 = mIndexData[i].VertexIndices[1];
				SE_UInt32 vi2 = mIndexData[i].VertexIndices[2];
				SE_UInt32 vi3 = mIndexData[i].VertexIndices[3];

				MetaMeshFaceIndex triangle0, triangle1;
				triangle0.VertexIndices.push_back(vi0);
				triangle0.VertexIndices.push_back(vi1);
				triangle0.VertexIndices.push_back(vi2);
				triangle1.VertexIndices.push_back(vi0);
				triangle1.VertexIndices.push_back(vi2);
				triangle1.VertexIndices.push_back(vi3);

				if( HasTCoord(0) )
				{
					SE_UInt32 ti0 = mIndexData[i].TCoordIndices[0];
					SE_UInt32 ti1 = mIndexData[i].TCoordIndices[1];
					SE_UInt32 ti2 = mIndexData[i].TCoordIndices[2];
					SE_UInt32 ti3 = mIndexData[i].TCoordIndices[3];
					triangle0.TCoordIndices.push_back(ti0);
					triangle0.TCoordIndices.push_back(ti1);
					triangle0.TCoordIndices.push_back(ti2);
					triangle1.TCoordIndices.push_back(ti0);
					triangle1.TCoordIndices.push_back(ti2);
					triangle1.TCoordIndices.push_back(ti3);
				}

                if( HasNormal() )
                {
                    SE_UInt32 vni0 = mIndexData[i].VertexNormalIndices[0];
                    SE_UInt32 vni1 = mIndexData[i].VertexNormalIndices[1];
                    SE_UInt32 vni2 = mIndexData[i].VertexNormalIndices[2];
                    SE_UInt32 vni3 = mIndexData[i].VertexNormalIndices[3];
                    triangle0.VertexNormalIndices.push_back(vni0);
                    triangle0.VertexNormalIndices.push_back(vni1);
                    triangle0.VertexNormalIndices.push_back(vni2);
                    triangle1.VertexNormalIndices.push_back(vni0);
                    triangle1.VertexNormalIndices.push_back(vni2);
                    triangle1.VertexNormalIndices.push_back(vni3);
                }

				newIndexData.push_back(triangle0);
				newIndexData.push_back(triangle1);
			}
			else
			{
				// TODO:
				// Only support converting quad face to triangle face for now.
				SE_ASSERT( false );
			}
		}

		mIndexData.clear();
		mIndexData = newIndexData;
		mFaceCount = (SE_Int32)mIndexData.size();
	}
	mFaceFlag = (SE_UInt32)MMFT_TriangleIndex;
}
//----------------------------------------------------------------------------
void SEObjMetaMesh::ConvertToUnified()
{
	if( mUnified )
	{
		return;
	}

	vector<VertexAttr>* vertexInfo = SE_NEW vector<VertexAttr>[mVertexCount];
	for( int i = 0; i < (int)mIndexData.size(); ++i )
	{
		MetaMeshFaceIndex& indices = mIndexData[i];
		int curFaceIndexCount = (int)indices.VertexIndices.size();

		for( int j = 0; j < curFaceIndexCount; ++j )
		{
			VertexAttr tempAttr;
			tempAttr.V = indices.VertexIndices[j];

			if( indices.TCoordIndices.size() > 0 )
			{
				tempAttr.T = indices.TCoordIndices[j];
			}

            if( indices.VertexNormalIndices.size() > 0 )
            {
                tempAttr.N = indices.VertexNormalIndices[j];
            }

			vertexInfo[tempAttr.V].push_back(tempAttr);
		}
	}

	SE_Int32 newVCount = 0;
	for( int i = 0; i < mVertexCount; ++i )
	{
		vector<VertexAttr>& curVertexInfo = vertexInfo[i];
		sort(curVertexInfo.begin(), curVertexInfo.end());
		vector<VertexAttr>::iterator itEnd = unique(curVertexInfo.begin(),
			curVertexInfo.end());
		curVertexInfo.erase(itEnd, curVertexInfo.end());
		newVCount += (SE_Int32)curVertexInfo.size();
	}

	vector<SEVector3f> newVertexData;
	newVertexData.resize(newVCount);

	vector<SEVector3f> newTCoordData;
	if( mTCoordData.size() > 0 )
	{
		newTCoordData.resize(newVCount);
	}

    vector<SEVector3f> newVertexNormalData;
    if( mVertexNormalData.size() > 0 )
    {
        newVertexNormalData.resize(newVCount);
    }

	for( int i = 0, k = 0; i < mVertexCount; ++i )
	{
		vector<VertexAttr>& curVertexInfo = vertexInfo[i];
		SE_Int32 vertexRefCount = (SE_Int32)curVertexInfo.size();

		for( int j = 0; j < vertexRefCount; ++j, ++k )
		{
			newVertexData[k] = mVertexData[i];

			VertexAttr tempAttr = curVertexInfo[j];

			if( newTCoordData.size() > 0 )
			{
				newTCoordData[k] = mTCoordData[tempAttr.T];
			}

            if( newVertexNormalData.size() > 0 )
            {
                newVertexNormalData[k] = mVertexNormalData[tempAttr.N];
            }

			tempAttr.V = k;
			curVertexInfo.push_back(tempAttr);
		}
	}

	for( int i = 0; i < (int)mIndexData.size(); ++i )
	{
		MetaMeshFaceIndex& indices = mIndexData[i];
		int curFaceIndexCount = (int)indices.VertexIndices.size();

		for( int j = 0; j < curFaceIndexCount; ++j )
		{
			VertexAttr tempAttr;
			tempAttr.V = indices.VertexIndices[j];

			if( indices.TCoordIndices.size() > 0 )
			{
				tempAttr.T = indices.TCoordIndices[j];
			}

            if( indices.VertexNormalIndices.size() > 0 )
            {
                tempAttr.N = indices.VertexNormalIndices[j];
            }

			vector<VertexAttr>& curVertexInfo = vertexInfo[tempAttr.V];
			int halfSize = (int)curVertexInfo.size() / 2;
			for( int k = 0; k < halfSize; ++k )
			{
				if( curVertexInfo[k] == tempAttr )
				{
					indices.VertexIndices[j] = curVertexInfo[halfSize + k].V;
					break;
				}
			}
		}
	}

	mVertexData = newVertexData;
	mVertexCount = (SE_Int32)mVertexData.size();
	mTCoordData = newTCoordData;
    mTCoordCount = (SE_Int32)mTCoordData.size();
    mVertexNormalData = newVertexNormalData;
    mVertexNormalCount = (SE_Int32)mVertexNormalData.size();
	mUnified = true;

	SE_DELETE[] vertexInfo;
}
//----------------------------------------------------------------------------