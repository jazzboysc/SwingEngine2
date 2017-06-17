// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RayTracingDeviceImage_H
#define Swing_RayTracingDeviceImage_H

#include "SERayTracingAdapterLIB.h"
#include "SEReferencable.h"
#include "SERayTracingDeviceResource.h"

namespace Swing
{

class SERayTracingDevice;
class SERayTracingDeviceBitmap;

enum SERTDeviceImageFileType
{
    RTDIFT_BMP,
    RTDIFT_JPG,
    RTDIFT_PNG,
    RTDIFT_MAX
};

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 04/05/2017
//----------------------------------------------------------------------------
class SE_RAY_TRACING_ADAPTER_API SERayTracingDeviceImage : public SEReferencable
{
public:
	SERayTracingDeviceImage();
	virtual ~SERayTracingDeviceImage();

    SERTImageHandle* GetImageHandle();
    void SetImageHandle(SERTImageHandle* imageHandle);

    SERayTracingDeviceBitmap* CreateRTBitmap(int width, int height);

    bool SaveToBmpFile(const std::string& fileName, bool preserveAlpha, bool invertChannels);

    int Width, Height;

protected:
	SERTImageHandle* mImageHandle;
};

typedef SESmartPointer<SERayTracingDeviceImage> SERayTracingDeviceImagePtr;

}

#endif