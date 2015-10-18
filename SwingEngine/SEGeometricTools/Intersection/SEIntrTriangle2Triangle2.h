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

#ifndef Swing_IntrTriangle2Triangle2_H
#define Swing_IntrTriangle2Triangle2_H

#include "SEGeometricToolsLIB.h"
#include "SEIntersector.h"
#include "SEIntersector1.h"
#include "SETriangle2.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081223
//----------------------------------------------------------------------------
class SE_GEOMETRICTOOLS_API SEIntrTriangle2Triangle2f : public SEIntersector<
    float, SEVector2f>
{
public:
    SEIntrTriangle2Triangle2f(const SETriangle2f& rTriangle0,
        const SETriangle2f& rTriangle1);

    // object access
    const SETriangle2f& GetTriangle0(void) const;
    const SETriangle2f& GetTriangle1(void) const;

    // static queries
    virtual bool Test(void);
    virtual bool Find(void);

    // dynamic queries
    virtual bool Test(float fTMax, const SEVector2f& rVelocity0,
        const SEVector2f& rVelocity1);
    virtual bool Find(float fTMax, const SEVector2f& rVelocity0,
        const SEVector2f& rVelocity1);

    // information about the intersection set
    int GetCount(void) const;
    const SEVector2f& GetPoint(int i) const;

private:
    static int OnWhichSide(const SEVector2f aV[3],
        const SEVector2f& rP, const SEVector2f& rD);

    static void ClipConvexPolygonAgainstLine(const SEVector2f& rN,
        float fC, int& riCount, SEVector2f aV[6]);

    enum ProjectionMap
    {
        M21,  // 2 vertices map to min, 1 vertex maps to max
        M12,  // 1 vertex maps to min, 2 vertices map to max
        M11   // 1 vertex maps to min, 1 vertex maps to max
    };

    class SEConfiguration
    {
    public:
        ProjectionMap Map;  // how vertices map to the projection interval
        int Index[3];       // the sorted indices of the vertices
        float Min, Max;     // the interval is [min,max]
    };

    void ComputeTwo(SEConfiguration& rCfg, const SEVector2f aV[3],
        const SEVector2f& rD, int iI0, int iI1, int iI2);

    void ComputeThree(SEConfiguration& rCfg, const SEVector2f aV[3],
        const SEVector2f& rD, const SEVector2f& rP);

    static bool NoIntersect(const SEConfiguration& rCfg0,
        const SEConfiguration& rCfg1, float fTMax, float fSpeed, int& riSide,
        SEConfiguration& rTCfg0, SEConfiguration& rTCfg1, float& rfTFirst,
        float& rfTLast);

    static void GetIntersection(const SEConfiguration& rCfg0,
        const SEConfiguration& rCfg1, int iSide, const SEVector2f aV0[3],
        const SEVector2f aV1[3], int& riCount, SEVector2f aVertex[6]);

    // the objects to intersect
    const SETriangle2f* m_pTriangle0;
    const SETriangle2f* m_pTriangle1;

    // information about the intersection set
    int m_iCount;
    SEVector2f m_aPoint[6];
};

}

#endif
