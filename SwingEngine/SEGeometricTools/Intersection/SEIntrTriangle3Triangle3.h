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

#ifndef Swing_IntrTriangle3Triangle3_H
#define Swing_IntrTriangle3Triangle3_H

#include "SEGeometricToolsLIB.h"
#include "SEIntersector.h"
#include "SETriangle3.h"
#include "SELine3.h"
#include "SEPlane3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081223
//----------------------------------------------------------------------------
class SE_GEOMETRICTOOLS_API SEIntrTriangle3Triangle3f : public SEIntersector<
    float, SEVector3f>
{
public:
    SEIntrTriangle3Triangle3f(const SETriangle3f& rTriangle0,
        const SETriangle3f& rTriangle1);

    // object access
    const SETriangle3f& GetTriangle0(void) const;
    const SETriangle3f& GetTriangle1(void) const;

    bool ReportCoplanarIntersections;  // default 'true'

    // static queries
    virtual bool Test(void);
    virtual bool Find(void);

    // dynamic queries
    virtual bool Test(float fTMax, const SEVector3f& rVelocity0,
        const SEVector3f& rVelocity1);
    virtual bool Find(float fTMax, const SEVector3f& rVelocity0,
        const SEVector3f& rVelocity1);

    // information about the intersection set
    int GetCount(void) const;
    const SEVector3f& GetPoint(int i) const;

private:
    static void ProjectOntoAxis(const SETriangle3f& rTri,
        const SEVector3f& rAxis, float& rfMin, float& rfMax);

    static void TrianglePlaneRelations(const SETriangle3f& rTriangle,
        const SEPlane3f& rPlane, float afDistance[3], int aiSign[3],
        int& riPositive, int& riNegative, int& riZero);

    static void GetInterval(const SETriangle3f& rTriangle,
        const SELine3f& rLine, const float afDistance[3],
        const int aiSign[3], float afParam[2]);

    bool GetCoplanarIntersection(const SEPlane3f& rPlane,
        const SETriangle3f& rTri0, const SETriangle3f& rTri1);

    static bool TestOverlap(float fTMax, float fSpeed, float fUMin, 
        float fUMax, float fVMin, float fVMax, float& rfTFirst, 
        float& rfTLast);

    bool TestOverlap(const SEVector3f& rAxis, float fTMax,
        const SEVector3f& rVelocity, float& rfTFirst, float& rfTLast);

    enum ProjectionMap
    {
        M2, M11,                // lines
        M3, M21, M12, M111,     // triangles
        M44, M2_2, M1_1         // boxes
    };

    enum ContactSide
    {
        CS_LEFT,
        CS_RIGHT,
        CS_NONE
    };

    class SE_GEOMETRICTOOLS_API SEConfiguration
    {
    public:
        ProjectionMap Map;  // how vertices map to the projection interval
        int Index[8];       // the sorted indices of the vertices
        float Min, Max;     // the interval is [min,max]
    };

    static void ProjectOntoAxis(const SETriangle3f& rTri,
        const SEVector3f& rAxis, SEConfiguration& rCfg);

    bool FindOverlap(float fTMax, float fSpeed, const SEConfiguration& rUC, 
        const SEConfiguration& rVC, ContactSide& reSide, SEConfiguration& rTUC, 
        SEConfiguration& rTVC, float& rfTFirst, float& rfTLast);

    bool FindOverlap(const SEVector3f& rAxis, float fTMax,
        const SEVector3f& rVelocity, ContactSide& reSide,
        SEConfiguration& rTCfg0, SEConfiguration& rTCfg1, float& rfTFirst,
        float& rfTLast);

    void FindContactSet(const SETriangle3f& rTri0,
        const SETriangle3f& rTri1, ContactSide& reSide,
        SEConfiguration& rCfg0, SEConfiguration& rCfg1);

    void GetEdgeEdgeIntersection(const SEVector3f& rU0,
        const SEVector3f& rU1, const SEVector3f& rV0,
        const SEVector3f& rV1);

    // the objects to intersect
    const SETriangle3f* m_pTriangle0;
    const SETriangle3f* m_pTriangle1;

    // information about the intersection set
    int m_iCount;
    SEVector3f m_aPoint[6];
};

}

#endif
