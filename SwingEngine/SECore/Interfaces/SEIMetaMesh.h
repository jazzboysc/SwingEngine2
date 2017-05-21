// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_IMetaMesh_H
#define Swing_IMetaMesh_H

#include "SECoreLIB.h"
#include "SEPlatforms.h"
#include "SEMatrix3.h"
#include "SEVector3.h"
#include "SEMatrix4.h"

namespace Swing
{

enum MetaMeshFaceType
{
    MMFT_Unknown = 0,
    MMFT_TriangleIndex = 1,
    MMFT_QuadIndex = 2
};

struct MetaMeshFaceIndex
{
    std::vector<SE_UInt32> VertexIndices;
    std::vector<SE_UInt32> TCoordIndices;
    std::vector<SE_UInt32> VertexNormalIndices;
};

//----------------------------------------------------------------------------
// Description:
// Date:20170521
//----------------------------------------------------------------------------
class SE_CORE_API SEIMetaMesh
{
public:
    virtual ~SEIMetaMesh() {}

    // Mesh data access interface.
    virtual std::vector<SEVector3f>& GetVertexData() = 0;
    virtual std::vector<SEVector3f>& GetTCoordData() = 0;
    virtual std::vector<SEVector3f>& GetVertexNormalData() = 0;
    virtual std::vector<SEVector3f>& GetFaceNormalData() = 0;
    virtual std::vector<MetaMeshFaceIndex>& GetIndexData() = 0;

protected:
    // Abstract interface.
    SEIMetaMesh() {}
};

}

#endif