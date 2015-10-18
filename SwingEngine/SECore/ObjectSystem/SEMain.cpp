// Swing Engine Version 2 Source Code 
// Most of techniques in the engine are mainly based on David Eberly's
// Wild Magic 4 and 5 open-source game engine. The author of Swing Engine 
// learned a lot from Eberly's experience of architecture and algorithm.
// Several subsystems are totally new, and others are reorganized or
// reimplimented based on Wild Magic's subsystems.
// Copyright (c) 2007-2011
//
// David Eberly's permission:
// Geometric Tools, LLC
// Copyright (c) 1998-2010
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt

#include "SECorePCH.h"
#include "SEMain.h"
#include "SEObject.h"

using namespace Swing;

SEMain::InitializerArray* SEMain::ms_pInitializers = 0;
SEMain::TerminatorArray* SEMain::ms_pTerminators = 0;
int SEMain::ms_iStartObjects = 0;
int SEMain::ms_iFinalObjects = 0;

//----------------------------------------------------------------------------
void SEMain::AddInitializer(Initializer FuncInitialize)
{
    if( !ms_pInitializers )
    {
        ms_pInitializers = SE_NEW InitializerArray;
    }

    ms_pInitializers->push_back(FuncInitialize);
}
//----------------------------------------------------------------------------
void SEMain::Initialize()
{
    bool bCountIsCorrect = true;

    // pre-main之前不允许创建object
    if( SEObject::InUse )
    {
        bCountIsCorrect = false;
        SEObject::PrintInUse("AppLog.txt", 
            "Objects were created before pre-main initialization");
    }
    SE_ASSERT( bCountIsCorrect );

    // 调用各个类型的pre-main函数
    if( ms_pInitializers )
    {
        for( int i = 0; i < (int)ms_pInitializers->size(); i++ )
        {
            (*ms_pInitializers)[i]();
        }
    }

    // 不再需要全局pre-main函数指针数组了
    SE_DELETE ms_pInitializers;
    ms_pInitializers = 0;

    // 在pre-main后创建的object数目
    if( SEObject::InUse )
    {
        ms_iStartObjects = SEObject::InUse->GetCount();
    }
    else
    {
        ms_iStartObjects = 0;
    }
}
//----------------------------------------------------------------------------
void SEMain::AddTerminator(Terminator FuncTerminate)
{
    if( !ms_pTerminators )
    {
        ms_pTerminators = SE_NEW TerminatorArray;
    }

    ms_pTerminators->push_back(FuncTerminate);
}
//----------------------------------------------------------------------------
void SEMain::Terminate()
{
    bool bCountIsCorrect = true;

    // 所有应用程序运行期间创建的object现在应该已经被delete
    if( SEObject::InUse )
    {
        ms_iFinalObjects = SEObject::InUse->GetCount();
    }
    else
    {
        ms_iFinalObjects = 0;
    }

    if( ms_iStartObjects != ms_iFinalObjects )
    {
        bCountIsCorrect = false;
        SEObject::PrintInUse("AppLog.txt", 
            "Not all objects were deleted before post-main termination");
    }

    // 调用各个类型的post-main函数
    if( ms_pTerminators )
    {
        int iCount = (int)ms_pTerminators->size();
        for( int i = 0; i < iCount; i++ )
        {
            (*ms_pTerminators)[i]();
        }
    }

    // 不再需要全局post-main函数指针数组了
    SE_DELETE ms_pTerminators;
    ms_pTerminators = 0;

    if( bCountIsCorrect )
    {
        // 所有在pre-main期间创建的object都应该在post-main期间释放
        if( SEObject::InUse )
        {
            ms_iFinalObjects = SEObject::InUse->GetCount();
        }
        else
        {
            ms_iFinalObjects = 0;
        }

        if( ms_iFinalObjects != 0 )
        {
            bCountIsCorrect = false;
            SEObject::PrintInUse("AppLog.txt", 
                "Objects were deleted after post-main termination");
        }
    }

    SE_ASSERT( bCountIsCorrect );

    // object级泄漏检测系统已经完成任务,
    // 释放该系统从而避免内存泄漏检测系统发现内存泄漏
    SE_DELETE SEObject::InUse;
    SEObject::InUse = 0;
}
//----------------------------------------------------------------------------
