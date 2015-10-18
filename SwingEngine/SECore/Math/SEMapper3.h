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

#ifndef Swing_Mapper3_H
#define Swing_Mapper3_H

#include "SECoreLIB.h"
#include "SEVector3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081202
//----------------------------------------------------------------------------
class SE_CORE_API SEMapper3f
{
public:
    // Construction and destruction.  The value of epsilon is used as a
    // relative error when computing the dimension of the point set.
    SEMapper3f(int iVCount, const SEVector3f* aVertex, float fEpsilon);
    ~SEMapper3f(void);

    // Axis-aligned bounding box of the input points.
    const SEVector3f& GetMin(void) const;
    const SEVector3f& GetMax(void) const;
    float GetMaxRange(void) const;

    // Dimension d of the set (0, 1, 2, or 3).
    int GetDimension(void) const;

    // Coordinate system.  The origin is valid for any dimension d.  The
    // unit-length direction vector is valid only for 0 <= i < d.  The extreme
    // index is relative to the array of input points, and is also valid only
    // for 0 <= i < d.  If d = 0, all points are effectively the same, but the
    // use of an epsilon may lead to an extreme index that is not zero.  If
    // d = 1, all points effectively lie on a line segment.  The extreme
    // indices correspond to input points that are the end points of the
    // segment.  If d = 2, the first two extreme indices correspond to a
    // line segment.  The next extreme index corresponds to the input point
    // that is farthest from this line segment in a direction perpendicular
    // to the segment.  If d = 3, the first three extreme indices correspond
    // to a triangle.  The next extreme index corresponds to the input point
    // that is farthest from this triangle in a direction perpendicular to
    // the triangle.
    const SEVector3f& GetOrigin(void) const;
    const SEVector3f& GetDirection(int i) const;
    int GetExtremeIndex(int i) const;

    // If d = 3, the direction vectors {U0,U1,U2} form a right-handed set.
    // The four extreme points form a tetrahedron.  This function indicates
    // if that tetrahedron is counterclockwise ordered.
    bool GetExtremeCCW(void) const;

private:
    // Axis-aligned bounding box of input points.  The maximum range is the
    // larger of max[0]-min[0], max[1]-min[1], and max[2]-min[2].
    SEVector3f m_Min, m_Max;
    float m_fMaxRange;

    // The intrinsic dimension of the input set.  The parameter fEpsilon to
    // the constructor is used to provide a tolerance when determining the
    // dimension.
    int m_iDimension;

    // The indices that define the maximum dimensional extents.  The values
    // m_aiExtreme[0] and m_aiExtreme[1] are the indices for the vertices
    // which define the largest extent in one of the coordinate axis
    // directions.  If the intrinsic dimensionality is 2, then m_aiExtreme[2]
    // is the index for the vertex which causes the largest extent in the
    // direction perpendicular to the line through the vertices corresponding
    // to m_aiExtreme[0] and m_aiExtreme[1].  Furthermore, if the intrinsic
    // dimensionality is 3, then m_aiExtreme[3] is the index for the vertex
    // which causes the largest extent in the direction perpendicular to the
    // triangle defined by the other extreme points.  The tetrahedron
    // <V[extreme0],V[extreme1],V[extreme2],V[extreme3]> can be clockwise or
    // counterclockwise, the condition stored in m_bExtremeCCW.
    int m_aiExtreme[4];
    bool m_bExtremeCCW;

    // See the comments describing the member functions which return these
    // values.
    SEVector3f m_Origin;
    SEVector3f m_aDirection[3];
};

}

#endif
