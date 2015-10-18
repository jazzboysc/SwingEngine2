// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_Voxelizer_H
#define Swing_Voxelizer_H

#include "SERenderingEngineLIB.h"
#include "SESubRenderer.h"
#include "SEAxisAlignedBox3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 02/23/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEVoxelizer : public SESubRenderer
{
public:
    enum SEVoxelizerType
    {
        VT_Grid = 0,
        VT_SVO,
        VT_Unknown
    };

    SEVoxelizer(SEGPUDevice* device, SERenderSet* renderSet = 0);
    virtual ~SEVoxelizer();

    void Render(int technique, int pass);

    int RasterizerDimBias;
    int VoxelGridDim;

    inline SEVoxelizerType GetVoxelizerType() const;
    float GetSceneBBMaxLength() const;

protected:
    void VoxelizeScene(int technique, int pass);

    SEVoxelizerType mVoxelizerType;
    SEAxisAlignedBox3f* mSceneBB;
    float mSceneBBMaxLength;
};

typedef SESmartPointer<SEVoxelizer> SEVoxelizerPtr;

#include "SEVoxelizer.inl"

}

#endif