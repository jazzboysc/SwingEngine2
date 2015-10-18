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

#ifndef Swing_Controller_H
#define Swing_Controller_H

#include "SECoreLIB.h"
#include "SEPlatforms.h"
#include "SEObject.h"

namespace Swing
{

class SEAdvancedObject;

//----------------------------------------------------------------------------
// Description:
// Date:20080327
//----------------------------------------------------------------------------
class SE_CORE_API SEController : public SEObject
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    virtual ~SEController(void);

    // ��ȡ�����ƵĶ���
    inline SEAdvancedObject* GetObject(void) const;

    // ��������ʵ�ִ˺���
    virtual bool Update(double dAppTime);

    enum RepeatType
    {
        RT_CLAMP,
        RT_WRAP,
        RT_CYCLE,
        RT_COUNT
    };

    RepeatType Repeat;  // default = RT_CLAMP
    double MinTime;     // default = 0.0
    double MaxTime;     // default = 0.0
    double Phase;       // default = 0.0
    double Frequency;   // default = 1.0
    bool Active;        // default = true

protected:
    // Abstract base class.
    SEController(void);

    friend class SEAdvancedObject;
    // ���غ�,������ɶ����������ı����ƶ����������ʶ��.
    virtual void SetObject(SEAdvancedObject* pObject);

    // ��applicationʱ�䵥λ��controllerʱ�䵥λ��ת��.
    // ����������update�����п���ʹ���������.
    double GetControlTime(double dAppTime);

    // ����ָ��ָ�򱻿��Ƶ�object,����object��controller��smart pointer
    // ��ָ����.
    SEAdvancedObject* mObject;

    double m_dLastAppTime;

private:
    static const char* ms_aacRepeatType[RT_COUNT];
};

typedef SESmartPointer<SEController> SEControllerPtr;

#include "SEController.inl"

}

#endif
