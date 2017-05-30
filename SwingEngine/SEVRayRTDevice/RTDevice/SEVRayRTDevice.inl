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
void SEVRayRTDevice::SetTransformHelper(T* sceneObject, SEMatrix3f* srcMatrix, SEVector3f* srcOffset)
{
    if( srcMatrix && srcOffset )
    {
        Matrix dstMatrix;
        Vector dstOffset;

        SECoordinateSystemAdapter::SEToZUpRHColumnMajorOrder<Matrix, Vector>(*srcMatrix, *srcOffset, dstMatrix, dstOffset);
        Transform trans(dstMatrix, dstOffset);
        sceneObject->set_transform(trans);
    }
    else
    {
        if( srcMatrix )
        {
            Matrix dstMatrix;
            Vector dstOffset(0.0f, 0.0f, 0.0f);

            SECoordinateSystemAdapter::SEToZUpRHColumnMajorOrder<Matrix>(*srcMatrix, dstMatrix);
            Transform trans(dstMatrix, dstOffset);
            sceneObject->set_transform(trans);
        }
        else
        {
            Matrix dstMatrix;
            dstMatrix.makeIdentity();
            Vector dstOffset;

            SECoordinateSystemAdapter::SEToZUpRHColumnMajorOrder<Vector>(*srcOffset, dstOffset);
            Transform trans(dstMatrix, dstOffset);
            sceneObject->set_transform(trans);
        }
    }
}
//----------------------------------------------------------------------------