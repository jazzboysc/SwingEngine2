// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

//----------------------------------------------------------------------------
template <class T>
void SEVRayRTDevice::SetLightCommon(SEILight* srcLight, T* dstLight)
{
    SEVector3f srcLoc = srcLight->GetLocation();
    SEMatrix3f srcRot = srcLight->GetRotation();
    Matrix dstRot;
    Vector dstLoc;

    SECoordinateSystemAdapter::SEToZUpRHColumnMajorOrder<Matrix, Vector>(srcRot, srcLoc, dstRot, dstLoc);
    Transform trans(dstRot, dstLoc);
    dstLight->set_transform(trans);

    SEColorRGB srcColor = srcLight->GetColor();
    Color dstColor;
    dstColor.r = srcColor.R;
    dstColor.g = srcColor.G;
    dstColor.b = srcColor.B;
    dstLight->set_color(dstColor);
}
//----------------------------------------------------------------------------