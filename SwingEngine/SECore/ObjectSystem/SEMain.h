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

#ifndef Swing_Main_H
#define Swing_Main_H

#include "SECoreLIB.h"
#include "SEPlatforms.h"
#include "SESystem.h"
#include "SEMainMCR.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:管理在应用程序启动前和结束后所需的所有操作,
//      一系列pre-main,post-main函数被调用用于创建和释放各个类型所需的全局数据,
//      pre-main,post-main函数由派生类根据各自需求实现
// Date:20080324
//----------------------------------------------------------------------------
class SE_CORE_API SEMain
{
public:
    typedef void (*Initializer)(void);  // 定义pre-main函数指针类型
    typedef std::vector<Initializer> InitializerArray; // 定义pre-main函数指针数组类型
    static void AddInitializer(Initializer FuncInitialize);
    static void Initialize(void);

    typedef void (*Terminator)(void); // 定义post-main函数指针类型
    typedef std::vector<Terminator> TerminatorArray; // 定义post-main函数指针数组类型
    static void AddTerminator(Terminator FuncTerminate);
    static void Terminate(void);

private:
    static InitializerArray* ms_pInitializers; // 全局pre-main函数指针数组
    static TerminatorArray* ms_pTerminators; // 全局post-main函数指针数组
    static int ms_iStartObjects;
    static int ms_iFinalObjects;
};

}

#endif
