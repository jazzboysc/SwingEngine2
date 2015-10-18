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

#ifndef Swing_GeometricToolsHeaders_H
#define Swing_GeometricToolsHeaders_H

// Geometric Tools Register
#include "SEGeometricToolsRegister.h"

// Approximation
#include "SEApprGaussPointsFit3.h"
#include "SEApprLineFit3.h"

// Collision

// Computational Geometry:  Convex Hull
#include "SEConvexHull.h"
#include "SEConvexHull1.h"
#include "SEConvexHull2.h"
#include "SEConvexHull3.h"
#include "SEHullEdge2.h"
#include "SEHullTriangle3.h"

// Computational Geometry:  Delaunay

// Computational Geometry:  Query
#include "SEQuery.h"
#include "SEQuery2.h"
#include "SEQuery2Filtered.h"
#include "SEQuery2Int64.h"
#include "SEQuery2TInteger.h"
#include "SEQuery2TRational.h"
#include "SEQuery3.h"
#include "SEQuery3Filtered.h"
#include "SEQuery3Int64.h"
#include "SEQuery3TInteger.h"
#include "SEQuery3TRational.h"

// Computational Geometry:  Rational Arithmetic
#include "SEIVector2.h"
#include "SEIVector3.h"
#include "SERVector2.h"
#include "SERVector3.h"
#include "SETInteger.h"
#include "SETIVector.h"
#include "SETRational.h"
#include "SETRVector.h"

// Computational Geometry:  Triangulate

// Containment
#include "SEContBox3.h"
#include "SEContSphere3.h"

// Distance
#include "SEDistance.h"
#include "SEDistLine2Line2.h"
#include "SEDistLine2Ray2.h"
#include "SEDistLine2Segment2.h"
#include "SEDistLine3Box3.h"
#include "SEDistLine3Line3.h"
#include "SEDistLine3Ray3.h"
#include "SEDistLine3Rectangle3.h"
#include "SEDistLine3Segment3.h"
#include "SEDistLine3Triangle3.h"
#include "SEDistRay2Ray2.h"
#include "SEDistRay2Segment2.h"
#include "SEDistRay3Box3.h"
#include "SEDistRay3Ray3.h"
#include "SEDistRay3Rectangle3.h"
#include "SEDistRay3Segment3.h"
#include "SEDistRay3Triangle3.h"
#include "SEDistRectangle3Rectangle3.h"
#include "SEDistSegment2Segment2.h"
#include "SEDistSegment3Box3.h"
#include "SEDistSegment3Rectangle3.h"
#include "SEDistSegment3Segment3.h"
#include "SEDistSegment3Triangle3.h"
#include "SEDistTriangle3Rectangle3.h"
#include "SEDistTriangle3Triangle3.h"
#include "SEDistVector2Box2.h"
#include "SEDistVector2Line2.h"
#include "SEDistVector2Ray2.h"
#include "SEDistVector2Segment2.h"
#include "SEDistVector3Box3.h"
#include "SEDistVector3Frustum3.h"
#include "SEDistVector3Line3.h"
#include "SEDistVector3Plane3.h"
#include "SEDistVector3Ray3.h"
#include "SEDistVector3Rectangle3.h"
#include "SEDistVector3Segment3.h"
#include "SEDistVector3Triangle3.h"

// Intersection
#include "SEIntersector.h"
#include "SEIntersector1.h"
#include "SEIntrBox3Box3.h"
#include "SEIntrLine3Box3.h"
#include "SEIntrLine3Sphere3.h"
#include "SEIntrRay3Box3.h"
#include "SEIntrRay3Sphere3.h"
#include "SEIntrRay3Triangle3.h"
#include "SEIntrSegment3Box3.h"
#include "SEIntrSegment3Sphere3.h"
#include "SEIntrSphere3Sphere3.h"
#include "SEIntrTriangle2Triangle2.h"
#include "SEIntrTriangle3Triangle3.h"

// Numerical Analysis
#include "SEEigen.h"
#include "SELinearSystem.h"

#endif