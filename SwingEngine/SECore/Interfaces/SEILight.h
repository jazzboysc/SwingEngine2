// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_ILight_H
#define Swing_ILight_H

#include "SECoreLIB.h"
#include "SEPlatforms.h"
#include "SEMatrix3.h"
#include "SEVector3.h"
#include "SEMatrix4.h"
#include "SEColorRGB.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20170513
//----------------------------------------------------------------------------
class SE_CORE_API SEILight
{
public:
    virtual ~SEILight() {}

    // MT form transformation interface.
    virtual SEMatrix3f GetRotation() const = 0;
    virtual SEVector3f GetLocation() const = 0;

    virtual SEColorRGB GetColor() const = 0;
    virtual float GetIntensity() const = 0;
    virtual SE_UInt32 GetSampleCount() const = 0;
    virtual bool CastShadow() const = 0;
    virtual bool IsVisible() const = 0;

protected:
    // Abstract interface.
    SEILight() {}
};

//----------------------------------------------------------------------------
// Description:
// Date:20170513
//----------------------------------------------------------------------------
class SE_CORE_API SEILightRectangle : public SEILight
{
public:
    virtual ~SEILightRectangle() {}

    virtual float GetWidth() const = 0;
    virtual float GetHeight() const = 0;

protected:
    // Abstract interface.
    SEILightRectangle() {}
};

//----------------------------------------------------------------------------
// Description:
// Date:20170513
//----------------------------------------------------------------------------
class SE_CORE_API SEISkyLight : public SEILight
{
public:
    virtual ~SEISkyLight() {}

    virtual bool IsSpherical() const = 0;
    virtual bool UseHDRFile() const = 0;
    virtual const char* GetHDRFilePath() const = 0;

protected:
    // Abstract interface.
    SEISkyLight() {}
};


}

#endif