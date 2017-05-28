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
template <class T>
void SEVRayRTDevice::SetTransformHelper(T* sceneObject, SEMatrix3f* srcRotation, SEVector3f* srcLocation)
{
    if( srcRotation && srcLocation )
    {
        Matrix dstRot;
        Vector dstLoc;

        SECoordinateSystemAdapter::SEToZUpRHColumnMajorOrder<Matrix, Vector>(*srcRotation, *srcLocation, dstRot, dstLoc);
        Transform trans(dstRot, dstLoc);
        sceneObject->set_transform(trans);
    }
    else
    {
        if( srcRotation )
        {
            Matrix dstRot;
            Vector dstLoc(0.0f, 0.0f, 0.0f);

            SECoordinateSystemAdapter::SEToZUpRHColumnMajorOrder<Matrix>(*srcRotation, dstRot);
            Transform trans(dstRot, dstLoc);
            sceneObject->set_transform(trans);
        }
        else
        {
            Matrix dstRot;
            dstRot.makeIdentity();
            Vector dstLoc;

            SECoordinateSystemAdapter::SEToZUpRHColumnMajorOrder<Vector>(*srcLocation, dstLoc);
            Transform trans(dstRot, dstLoc);
            sceneObject->set_transform(trans);
        }
    }
}
//----------------------------------------------------------------------------