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

#ifndef Swing_MainMCR_H
#define Swing_MainMCR_H

// ʹ���߱���ʵ�ָ��Ե�Initialize����,�ú�����Ӧ�ó���pre-main�ڼ䱻ϵͳ����.
// ʹ���߱���ʵ�ָ��Ե�Terminate����,�ú�����Ӧ�ó���post-main�ڼ䱻ϵͳ����.
//----------------------------------------------------------------------------
#define SE_DECLARE_INITIALIZE \
public: \
    static bool RegisterInitialize(void); \
    static void Initialize(void); \
private: \
    static bool ms_bInitializeRegistered
//----------------------------------------------------------------------------
#define SE_IMPLEMENT_INITIALIZE(classname) \
bool classname::ms_bInitializeRegistered = false; \
bool classname::RegisterInitialize() \
{ \
    if( !ms_bInitializeRegistered ) \
    { \
        SEMain::AddInitializer(classname::Initialize); \
        ms_bInitializeRegistered = true; \
    } \
    return ms_bInitializeRegistered; \
}
//----------------------------------------------------------------------------
#define SE_REGISTER_INITIALIZE(classname) \
static bool gs_bInitializeRegistered_##classname = \
    classname::RegisterInitialize()
//----------------------------------------------------------------------------
#define SE_INVOKE_INITIALIZE(classname) \
(void)gs_bInitializeRegistered_##classname
//----------------------------------------------------------------------------
#define SE_DECLARE_TERMINATE \
public: \
    static bool RegisterTerminate(void); \
    static void Terminate(void); \
private: \
    static bool ms_bTerminateRegistered
//----------------------------------------------------------------------------
#define SE_IMPLEMENT_TERMINATE(classname) \
bool classname::ms_bTerminateRegistered = false; \
bool classname::RegisterTerminate() \
{ \
    if( !ms_bTerminateRegistered ) \
    { \
        SEMain::AddTerminator(classname::Terminate); \
        ms_bTerminateRegistered = true; \
    } \
    return ms_bTerminateRegistered; \
}
//----------------------------------------------------------------------------
#define SE_REGISTER_TERMINATE(classname) \
static bool gs_bTerminateRegistered_##classname = \
    classname::RegisterTerminate()
//----------------------------------------------------------------------------
#define SE_INVOKE_TERMINATE(classname) \
(void)gs_bTerminateRegistered_##classname
//----------------------------------------------------------------------------

#endif
