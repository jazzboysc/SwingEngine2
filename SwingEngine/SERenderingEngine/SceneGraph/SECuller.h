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

#ifndef Swing_Culler_H
#define Swing_Culler_H

#include "SERenderingEngineLIB.h"
#include "SECamera.h"
#include "SEPlane3.h"
#include "SESpatial.h"
#include "SEUnculledSet.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20080707
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SECuller
{
public:
    // ǰ�����������ڴ���Ǳ�ڿ�������������.
    // ���û�д���camera,�Ժ�Ӧ��ʹ��ComputeUnculledSet֮ǰ,
    // ͨ��SetCamera����camera.
    SECuller(int iMaxCount = 0, int iGrowBy = 0, const SECamera* pCamera = 0);
    virtual ~SECuller(void);

    // ����camera,frustum copy,Ǳ�ڿ�������������.
    inline void SetCamera(const SECamera* pCamera);
    inline const SECamera* GetCamera(void) const;
    void SetFrustum(const float* afFrustum);
    inline const float* GetFrustum(void) const;
    inline SEUnculledSet& GetVisibleSet(void);
    inline SEUnculledSet& GetAudibleSet(void);

    // ����Ĭ����Ϊ�Ǹ��ݴ����������һ��SEUnculledObject,
    // ��������ӵ�SEUnculledObject����ĩβ.
    // ������������ش˺���,����SEUnculledObject������Ա�ά��Ϊ��������,
    // �Ӷ�ʹ��Ⱦ״̬�ı���С��,
    // ����Ҳ���Գ�Ϊһ������unique list��portalϵͳʹ��.
    virtual void Insert(SESpatial* pObject, bool bIsAudible = false);

    // ����ռ��޳�ƽ���ջ����.
    // �û������ڽ�Ͷ��ƽ�����������û��Զ���ƽ��,
    // PushPlane�����������������ռ�ƽ��.
    enum { US_MAX_PLANE_COUNT = 32 };
    inline int GetPlaneCount(void) const;
    inline const SEPlane3f* GetPlanes(void) const;
    inline void SetPlaneState(unsigned int uiPlaneState);
    inline unsigned int GetPlaneState(void) const;
    inline void PushPlane(const SEPlane3f& rPlane);
    inline void PopPlane(void);

    // �Ѷ��������ռ�BV�͸����޳�ƽ��Ƚ�,
    // ����¼����ƽ����֮�ȽϵĽ��,���޳�ϵͳ�ڳ����޳�����������ʹ��.
    // ֻ��SESpatial����Ҫ�����������.
    bool IsInFrustum(const SEBoundingVolume* pBound);

    // ֧��Portal::GetUnculledSet.
    bool IsInFrustum(int iVertexCount, const SEVector3f* aVertex,
        bool bIgnoreNearPlane);

    // �Ѷ��������ռ�BV�͸����޳�ƽ��Ƚ�,
    // ֻ�жϸö����Ƿ���frustum��,����¼�ȽϽ��.
    // ֧��SERoamTerrainPage::Simplify.
    bool IsSingleInFrustum(const SEBoundingVolume* pBound) const;

    // ֧��BspNode::GetUnculledSet.
    // �жϽ�Ͷ����ƽ���λ�����,
    // ��ƽ������ռ䷵��+1,����ռ䷵��-1,�ཻ����0.
    // ����ƽ��Ϊ����ռ�ƽ��.
    int OnWhichSide(const SEPlane3f& rPlane) const;

    // �޳�ϵͳ���.
    // ͨ����������ĳ�����ͼ��,����Ǳ�ڿ��������󼯺�.
    void ComputeUnculledSet(SESpatial* pScene);

protected:
    // �޳�ϵͳ��Ҫ֪����ǰ������������Ϣ.
    const SECamera* m_pCamera;

    // �����������Ͷ������Ŀ���ֵ.
    // �⽫���������ϵͳ���޳�������(����portalϵͳ),
    // �ı��Ͷ���������Ӱ�쵱ǰ�����.
    float m_Frustum[SECamera::VF_COUNT];

    // ����ռ�ƽ������,����6����Ͷ��ƽ��,����26���û��Զ���ƽ��.
    // m_uiPlaneState��32λ��Ӧ32���޳�ƽ���״̬,
    // ״̬λΪ1ʱ������ƽ��Ӧ�ò����޳��Ƚ�,Ϊ0ʱ���ò���.
    // ��������Ŀ����������޳�ϵͳ��Ч��,
    // ����ĳ���ڵ��BV��ȫ����ĳƽ���һ����ռ���,
    // ��ɶ϶��������ӽڵ�Ҳ�����ڸ�ƽ��ĸð�ռ���,
    // �Ӷ������ӽڵ���ظ����޳��Ƚ�.
    int m_iPlaneCount;
    SEPlane3f m_aPlane[US_MAX_PLANE_COUNT];
    unsigned int m_uiPlaneState;

    // ����ComputeUnculledSet�������Ǳ�ڿ��Ӷ��󼯺�.
    SEUnculledSet m_VisibleSet;

    // ����ComputeUnculledSet�������Ǳ�ڿ������󼯺�.
    SEUnculledSet m_AudibleSet;
};

#include "SECuller.inl"

}

#endif
