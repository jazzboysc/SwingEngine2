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

#ifndef Swing_CoreHeaders_H
#define Swing_CoreHeaders_H

// Core Register
#include "SECoreRegister.h"

// Base
#include "SEHashSet.h"
#include "SEHashTable.h"
#include "SEPriorityQueue.h"
#include "SEStringHashTable.h"
#include "SETuple.h"
#include "SEBlockedArray.h"
#include "SEKdTree.h"

// Math
#include "SEAxisAlignedBox2.h"
#include "SEAxisAlignedBox3.h"
#include "SEBitHacks.h"
#include "SEBox2.h"
#include "SEBox3.h"
#include "SEColor.h"
#include "SEColorRGB.h"
#include "SEColorRGBA.h"
#include "SEConvexPolyhedron3.h"
#include "SEFloat1.h"
#include "SEFloat2.h"
#include "SEFloat3.h"
#include "SEFloat4.h"
#include "SEFrustum3.h"
#include "SEHalfFloat.h"
#include "SELinComp.h"
#include "SELine2.h"
#include "SELine3.h"
#include "SEMapper2.h"
#include "SEMapper3.h"
#include "SEMath.h"
#include "SEMatrix2.h"
#include "SEMatrix3.h"
#include "SEMatrix4.h"
#include "SEMatrixMN.h"
#include "SEPlane3.h"
#include "SEPolyhedron3.h"
#include "SEQuaternion.h"
#include "SERay2.h"
#include "SERay3.h"
#include "SERectangle3.h"
#include "SESegment2.h"
#include "SESegment3.h"
#include "SESphere3.h"
#include "SETriangle2.h"
#include "SETriangle3.h"
#include "SEVector2.h"
#include "SEVector3.h"
#include "SEVector4.h"
#include "SEVectorN.h"
#include "SETransformation.h"
#include "SERandomNumberGenerator.h"
#include "SERayDifferential.h"
#include "SEMonteCarlo.h"
#include "SEDistribution1D.h"
#include "SEDistribution2D.h"
#include "SEPermutedHalton.h"
#include "SECoordinateSystemAdapter.h"

// Object System
#include "SEReferencable.h"
#include "SEAdvancedObject.h"
#include "SEMain.h"
#include "SEObject.h"
#include "SERTTI.h"
#include "SESmartPointer.h"
#include "SEStream.h"
#include "SEStreamVersion.h"
#include "SEController.h"

// Threading
#include "SEMutex.h"
#include "SEMutexType.h"
#include "SEAtomicOperation.h"
#include "SERWMutex.h"
#include "SERWMutexType.h"
#include "SESemaphore.h"
#include "SESemaphoreType.h"
#include "SEConditionVariable.h"
#include "SEConditionVariableType.h"
#include "SETask.h"
#include "SETaskManager.h"
#include "SEThread.h"
#include "SEThreadType.h"
#include "SETaskWorkerThread.h"

// System
#include "SEAssert.h"
#include "SECommand.h"
#include "SEMemory.h"
#include "SEPlatforms.h"
#include "SESystem.h"
#include "SEMemoryArena.h"
#include "SETimer.h"
#include "SEProgressReporter.h"

// Shared Arrays
#include "SEColorRGBAArray.h"
#include "SEColorRGBArray.h"
#include "SEDoubleArray.h"
#include "SEFloatArray.h"
#include "SEIntArray.h"
#include "SEMatrix2Array.h"
#include "SEMatrix3Array.h"
#include "SEMatrix4Array.h"
#include "SEPlaneArray.h"
#include "SEQuaternionArray.h"
#include "SESharedArray.h"
#include "SEVector2Array.h"
#include "SEVector3Array.h"
#include "SEVector4Array.h"

// Interfaces
#include "SEICamera.h"
#include "SEILight.h"
#include "SEIMetaMesh.h"

#endif