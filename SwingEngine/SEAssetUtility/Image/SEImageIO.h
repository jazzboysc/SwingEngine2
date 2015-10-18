// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_ImageIO_H
#define Swing_ImageIO_H

#include "SEAssetUtilityLIB.h"
#include <string>

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20130603
//----------------------------------------------------------------------------
class SE_ASSET_UTILITY_API SEImageIO
{
public:
    static void WriteImage(const std::string& name, float* pixels, 
        float* alpha, int xRes, int yRes, int totalXRes, int totalYRes, 
        int xOffset, int yOffset);

    static void WriteImageTGA(const std::string& name, float* pixels,
        float* alpha, int xRes, int yRes, int totalXRes, int totalYRes,
        int xOffset, int yOffset);
};

}

#endif