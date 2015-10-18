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

#ifndef Swing_Spatial_H
#define Swing_Spatial_H

#include "SERenderingEngineLIB.h"
#include "SEPlatforms.h"
#include "SEAdvancedObject.h"
#include "SEBoundingVolume.h"
#include "SETransformation.h"

namespace Swing
{

class SECuller;

//----------------------------------------------------------------------------
// Description:
// Date:20080707
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SESpatial : public SEAdvancedObject
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    // Abstract base class.
    virtual ~SESpatial(void);

    // Local��World�任.
    // ĳЩ�������Ҳ����Ҫ����SESpatial::UpdateGS()ģ��Ӷ�ֱ������world�任,
    // ��ʱӦ����WorldIsCurrentΪtrue.
    // ����IK controller��Skin controller����Ҫ��������.
    SETransformation Local;
    SETransformation World;
    bool WorldIsCurrent;

    // ����ռ�BV.
    // ĳЩ�������Ҳ����Ҫ����SESpatial::UpdateGS()ģ��Ӷ�ֱ����������ռ�BV,
    // ��ʱӦ����WorldBoundIsCurrentΪtrue.
    SEBoundingVolumePtr WorldBound;
    bool WorldBoundIsCurrent;

    // ע��:
    // SESpatial����Ҫ����ģ�Ϳռ�BV.

    // �޳�ģʽ
    enum CullingMode
    {
        // ��������ռ�BV������ռ��޳�ƽ��,���ж�̬�޳�.
        CULL_DYNAMIC,

        // ǿ���޳�,�����һ��SENode�ڵ�,�������������������޳�.
        CULL_ALWAYS,

        // ǿ�Ʋ��޳�,�����һ��SENode�ڵ�,�������������������޳�.
        // ���״�������SENode�ڵ��,bNoCull����������Ϊtrue,
        // �����ݸ�GetUnculledSet/OnGetUnculledSet AB�ݹ���,
        // �Ӷ�ʹ�ݹ�·���ϵ������ӽڵ㶼���ᱻ�޳�.
        CULL_NEVER,

        MAX_CULLING_MODE
    };

    CullingMode Culling;

    // Geometric state��Controller�������.
    // UpdateGS�������¼���ÿ���ڵ������任,���ϼ�������BV.
    // UpdateBS����ֻ���ϼ�������BV,��ģ�����ݸı�ʱ,ֻ��ı�ģ��BV������BV,
    // �������¼���ռ�任.
    void UpdateGS(double dAppTime = -SEMathd::MAX_REAL,
        bool bInitiator = true);
    void UpdateBS(void);

    // Parent access.
    inline SESpatial* GetParent(void);

protected:
    // Streaming support.
    SESpatial(void);

    // Support for the geometric update.
    // ������SENodeʵ�����½���AB�ݹ�.
    // ���ϼ�������ռ�BV,������ʵ�־�����Ϊ.
    virtual void UpdateWorldData(double dAppTime);
    virtual void UpdateWorldBound(void) = 0;
    void PropagateBoundToRoot(void);

    SESpatial* m_pParent;

private:
    static const char* ms_aacCullingMode[MAX_CULLING_MODE];

// Internal use
public:
    inline void SetParent(SESpatial* pParent);

    // Support for hierarchical culling.
    void OnGetUnculledSet(SECuller& rCuller, bool bNoCull);
    virtual void GetUnculledSet(SECuller& rCuller, bool bNoCull) = 0;
};

typedef SESmartPointer<SESpatial> SESpatialPtr;

#include "SESpatial.inl"

}

#endif
