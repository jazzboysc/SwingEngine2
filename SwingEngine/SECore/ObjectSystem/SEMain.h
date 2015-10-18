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
// Description:������Ӧ�ó�������ǰ�ͽ�������������в���,
//      һϵ��pre-main,post-main�������������ڴ������ͷŸ������������ȫ������,
//      pre-main,post-main��������������ݸ�������ʵ��
// Date:20080324
//----------------------------------------------------------------------------
class SE_CORE_API SEMain
{
public:
    typedef void (*Initializer)(void);  // ����pre-main����ָ������
    typedef std::vector<Initializer> InitializerArray; // ����pre-main����ָ����������
    static void AddInitializer(Initializer FuncInitialize);
    static void Initialize(void);

    typedef void (*Terminator)(void); // ����post-main����ָ������
    typedef std::vector<Terminator> TerminatorArray; // ����post-main����ָ����������
    static void AddTerminator(Terminator FuncTerminate);
    static void Terminate(void);

private:
    static InitializerArray* ms_pInitializers; // ȫ��pre-main����ָ������
    static TerminatorArray* ms_pTerminators; // ȫ��post-main����ָ������
    static int ms_iStartObjects;
    static int ms_iFinalObjects;
};

}

#endif
