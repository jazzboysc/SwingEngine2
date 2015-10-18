#ifndef RTGI_GridVoxelizer_H
#define RTGI_GridVoxelizer_H

#include "SwingEngineRenderingEngine.h"
#include "SEVoxelizer.h"

namespace Swing
{

#define RTGI_GridVoxelizer_VoxelBuffer_Name "VoxelBuffer"

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/05/2014
//----------------------------------------------------------------------------
class ResetVoxelBuffer : public SEComputeTask
{
public:
    ResetVoxelBuffer();
    ~ResetVoxelBuffer();

    // Implement base class interface.
    virtual void OnGetShaderConstants();
    virtual void OnPreDispatch(unsigned int pass);
    virtual void OnPostDispatch(unsigned int pass);
};

typedef SESmartPointer<ResetVoxelBuffer> ResetVoxelBufferPtr;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 12/05/2014
//----------------------------------------------------------------------------
class GridVoxelizer : public SEVoxelizer
{
public:
    GridVoxelizer(SEGPUDevice* device, SERenderSet* renderSet = 0);
    virtual ~GridVoxelizer();

    void Initialize(SEGPUDevice* device, int voxelGridDim, 
        int voxelGridLocalGroupDim, SEAxisAlignedBox3f* sceneBB);

    // Implement base class interface.
    void OnRender(int technique, int pass, SERTGICamera* camera);

private:
    int mVoxelGridLocalGroupDim;
    int mGlobalDim;
    ResetVoxelBufferPtr mResetVoxelBufferTask;
};

typedef SESmartPointer<GridVoxelizer> GridVoxelizerPtr;

}

#endif