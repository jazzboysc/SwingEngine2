// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

//TODO: it is technically windows appication right now
#include "SEApplicationPCH.h"
#include "SEApplication.h"
#include "SERTGICamera.h"
#include "SETextureManager.h"
#include "SEVector2.h"
#include "SEVector3.h"

using namespace Swing;

SEApplication* SEApplication::mInstance = 0;

//----------------------------------------------------------------------------
SEApplication::SEApplication()
	:
    FrameCounter(0),
	mMainCameraSpeed(5.0f),
    mDevice(0),
    mMainCamera(0)
{
    SE_ASSERT( mInstance == 0 );
    mInstance = this;
}
//----------------------------------------------------------------------------
SEApplication::~SEApplication()
{
    if( mDevice )
    {
        mDevice->Terminate();
        mDevice = 0;
    }
}
//----------------------------------------------------------------------------
SEApplication* SEApplication::GetInstance()
{
    return mInstance;
}
//----------------------------------------------------------------------------