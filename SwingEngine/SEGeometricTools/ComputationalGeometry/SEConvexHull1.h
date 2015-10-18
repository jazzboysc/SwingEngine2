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

#ifndef Swing_ConvexHull1_H
#define Swing_ConvexHull1_H

// A fancy class to compute the minimum and maximum of a collection of
// real-valued numbers, but this provides some convenience for SEConvexHull2f 
// and SEConvexHull3f when the input point set has intrinsic dimension smaller
// than the containing space. The interface of SEConvexHull1f is also the 
// model for those of SEConvexHull2f and SEConvexHull3f.

#include "SEGeometricToolsLIB.h"
#include "SEConvexHull.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081201
//----------------------------------------------------------------------------
class SE_GEOMETRICTOOLS_API SEConvexHull1f : public SEConvexHullf
{
public:
    // The input to the constructor is the array of vertices you want to sort.
    // If you want SEConvexHull1f to delete the array during destruction,  set
    // bOwner to 'true'.  Otherwise,  you own the array and must delete it
    // yourself.  TO DO:  The computation type is currently ignored by this
    // class.  Add support for the various types later.
    SEConvexHull1f(int iVertexCount,  float* afVertex,  float fEpsilon, 
        bool bOwner,  SEQuery::Type eQueryType);
    virtual ~SEConvexHull1f(void);

    // The input vertex array.
    const float* GetVertices(void) const;

    // Support for streaming to/from disk.
    SEConvexHull1f(const char* acFilename);
    bool Load(const char* acFilename);
    bool Save(const char* acFilename) const;

private:
    float* m_afVertex;

    class SE_GEOMETRICTOOLS_API SESortedVertex
    {
    public:
        float Value;
        int Index;

        bool operator < (const SESortedVertex& rProj) const
        {
            return Value < rProj.Value;
        }
    };
};

}

#endif
