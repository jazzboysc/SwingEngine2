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

#ifndef Swing_ConvexHull_H
#define Swing_ConvexHull_H

#include "SEGeometricToolsLIB.h"
#include "SESystem.h"
#include "SEQuery.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081201
//----------------------------------------------------------------------------
class SE_GEOMETRICTOOLS_API SEConvexHullf
{
public:
    // Abstract base class.
    virtual ~SEConvexHullf(void);

    // Member accessors.  For notational purposes in this class documentation, 
    // The number of vertices is VQ and the vertex array is V.
    int GetQueryType(void) const;
    int GetVertexCount(void) const;
    float GetEpsilon(void) const;
    bool GetOwner(void) const;

    // The dimension of the result,  call it d.  If n is the dimension of the
    // space of the input points,  then 0 <= d <= n.
    int GetDimension(void) const;

    // The interpretations of the return values of these functions depends on
    // the dimension.  Generally,  SC = GetSimplexCount() is the number of
    // simplices in the mesh.  The array I is returned by GetIndices().
    int GetSimplexCount(void) const;
    const int* GetIndices(void) const;

    // Dimension d = 0.
    //   SC = 1
    //   I  = null (use index zero for vertices)

    // Dimension d = 1.
    //   SC = 2
    //   I = array of two indices
    // The segment has end points
    //   vertex[0] = V[I[2*i+0]]
    //   vertex[1] = V[I[2*i+1]]

    // Dimension d = 2.
    //   SC = number of convex polygon edges
    //   I  = array of into V that represent the convex polygon edges
    //        (SC total elements)
    // The i-th edge has vertices
    //   vertex[0] = V[I[2*i+0]]
    //   vertex[1] = V[I[2*i+1]]

    // Dimension d = 3.
    //   SC = number of convex polyhedron triangular faces
    //   I  = array of 3-tuples of indices into V that represent the
    //        triangles (3*SC total elements)
    // The i-th face has vertices
    //   vertex[0] = V[I[3*i+0]]
    //   vertex[1] = V[I[3*i+1]]
    //   vertex[2] = V[I[3*i+2]]

protected:
    // Abstract base class. The number of vertices to be processed is
    // iVCount. The value of fEpsilon is a tolerance used for determining
    // the intrinsic dimension of the input set of vertices. Ownership of the
    // input points to the constructors for the derived classes may be
    // transferred to this class. If you want the input vertices to be
    // deleted by this class, set bOwner to 'true'; otherwise, you own the
    // array and must delete it yourself.
    SEConvexHullf(int iVertexCount,  float fEpsilon,  bool bOwner, 
        SEQuery::Type eQueryType);

    // Support for streaming to/from disk.
    bool Load(FILE* pIFile);
    bool Save(FILE* pOFile) const;

    SEQuery::Type m_eQueryType;
    int m_iVertexCount;
    int m_iDimension;
    int m_iSimplexCount;
    int* m_aiIndex;
    float m_fEpsilon;
    bool m_bOwner;
};

}

#endif
