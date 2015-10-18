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

#include "SECorePCH.h"
#include "SECoreRegister.h"
#include "SECoreHeaders.h"

using namespace Swing;

//----------------------------------------------------------------------------
void Swing::SE_Core_Register()
{
    // Object System
    SE_REGISTER_STREAM(SEAdvancedObject);
    SE_REGISTER_STREAM(SEObject);
	SE_REGISTER_STREAM(SEController);

    SE_INVOKE_STREAM(SEAdvancedObject);
    SE_INVOKE_STREAM(SEObject);
    SE_INVOKE_STREAM(SEController);

    // Shared Arrays
    SE_REGISTER_STREAM(SEColorRGBAArray);
    SE_REGISTER_STREAM(SEColorRGBArray);
    SE_REGISTER_STREAM(SEDoubleArray);
    SE_REGISTER_STREAM(SEFloatArray);
    SE_REGISTER_STREAM(SEIntArray);
    SE_REGISTER_STREAM(SEMatrix2fArray);
    SE_REGISTER_STREAM(SEMatrix3fArray);
    SE_REGISTER_STREAM(SEMatrix4fArray);
    SE_REGISTER_STREAM(SEPlane3fArray);
    SE_REGISTER_STREAM(SEQuaternionfArray);
    SE_REGISTER_STREAM(SEVector2fArray);
    SE_REGISTER_STREAM(SEVector3fArray);
    SE_REGISTER_STREAM(SEVector4fArray);

    SE_INVOKE_STREAM(SEColorRGBAArray);
    SE_INVOKE_STREAM(SEColorRGBArray);
    SE_INVOKE_STREAM(SEDoubleArray);
    SE_INVOKE_STREAM(SEFloatArray);
    SE_INVOKE_STREAM(SEIntArray);
    SE_INVOKE_STREAM(SEMatrix2fArray);
    SE_INVOKE_STREAM(SEMatrix3fArray);
    SE_INVOKE_STREAM(SEMatrix4fArray);
    SE_INVOKE_STREAM(SEPlane3fArray);
    SE_INVOKE_STREAM(SEQuaternionfArray);
    SE_INVOKE_STREAM(SEVector2fArray);
    SE_INVOKE_STREAM(SEVector3fArray);
    SE_INVOKE_STREAM(SEVector4fArray);
}
//----------------------------------------------------------------------------