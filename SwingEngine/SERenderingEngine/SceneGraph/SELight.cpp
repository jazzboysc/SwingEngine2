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

#include "SERenderingEnginePCH.h"
#include "SELight.h"
#include "SEMatrix3.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SELight, SEAdvancedObject);
SE_IMPLEMENT_STREAM(SELight);
SE_IMPLEMENT_DEFAULT_NAME_ID(SELight, SEAdvancedObject);

//----------------------------------------------------------------------------
SELight::SELight(LightType eType)
    :
    Type(eType),
    Ambient(0.0f, 0.0f, 0.0f),
    Diffuse(0.0f, 0.0f, 0.0f),
    Specular(0.0f, 0.0f, 0.0f),
    Position(0.0f, 0.0f, 0.0f),
    RVector(1.0f, 0.0f, 0.0f),
    UVector(0.0f, 1.0f, 0.0f),
    DVector(0.0f, 0.0f, 1.0f)
{
    Constant = 1.0f;
    Linear = 0.0f;
    Quadratic = 0.0f;
    Intensity = 1.0f;
    Angle = SEMathf::PI;
    CosAngle = -1.0f;
    SinAngle = 0.0f;
    Exponent = 1.0f;
}
//----------------------------------------------------------------------------
SELight::~SELight()
{
}
//----------------------------------------------------------------------------
void SELight::SetAngle(float fAngle)
{
    SE_ASSERT( 0.0f < fAngle && fAngle <= SEMathf::PI );

    Angle = fAngle;
    CosAngle = SEMathf::Cos(fAngle);
    SinAngle = SEMathf::Sin(fAngle);
}
//----------------------------------------------------------------------------
void SELight::SetDirection(const SEVector3f& rDirection, bool bUnitLength)
{
    DVector = rDirection;
    SEVector3f::GetOrthonormalBasis(UVector, RVector, DVector, bUnitLength);
}
//----------------------------------------------------------------------------
bool SELight::IsValidFrame() const
{
    float fTest = DVector.Dot(UVector);
    if( SEMathf::FAbs(fTest) > SEMathf::ZERO_TOLERANCE )
    {
        return false;
    }

    fTest = DVector.Dot(RVector);
    if( SEMathf::FAbs(fTest) > SEMathf::ZERO_TOLERANCE )
    {
        return false;
    }

    fTest = UVector.Dot(RVector);
    if( SEMathf::FAbs(fTest) > SEMathf::ZERO_TOLERANCE )
    {
        return false;
    }

    fTest = DVector.Dot(RVector.Cross(UVector));

    return SEMathf::FAbs(1.0f - fTest) <= SEMathf::ZERO_TOLERANCE;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// streaming
//----------------------------------------------------------------------------
void SELight::Load(SEStream& rStream, SEStream::SELink* pLink)
{
    SE_BEGIN_DEBUG_STREAM_LOAD;

    SEAdvancedObject::Load(rStream, pLink);

    // native data
    rStream.Read((int&)Type);
    rStream.Read(Position);
    rStream.Read(RVector);
    rStream.Read(UVector);
    rStream.Read(DVector);
    rStream.Read(Ambient);
    rStream.Read(Diffuse);
    rStream.Read(Specular);
    rStream.Read(Intensity);
    rStream.Read(Constant);
    rStream.Read(Linear);
    rStream.Read(Quadratic);
    rStream.Read(Angle);
    rStream.Read(CosAngle);
    rStream.Read(SinAngle);
    rStream.Read(Exponent);

    SE_END_DEBUG_STREAM_LOAD(SELight);
}
//----------------------------------------------------------------------------
void SELight::Link(SEStream& rStream, SEStream::SELink* pLink)
{
    SEAdvancedObject::Link(rStream, pLink);
}
//----------------------------------------------------------------------------
bool SELight::Register(SEStream& rStream) const
{
    return SEAdvancedObject::Register(rStream);
}
//----------------------------------------------------------------------------
void SELight::Save(SEStream& rStream) const
{
    SE_BEGIN_DEBUG_STREAM_SAVE;

    SEAdvancedObject::Save(rStream);

    // native data
    rStream.Write((int)Type);
    rStream.Write(Position);
    rStream.Write(RVector);
    rStream.Write(UVector);
    rStream.Write(DVector);
    rStream.Write(Ambient);
    rStream.Write(Diffuse);
    rStream.Write(Specular);
    rStream.Write(Intensity);
    rStream.Write(Constant);
    rStream.Write(Linear);
    rStream.Write(Quadratic);
    rStream.Write(Angle);
    rStream.Write(CosAngle);
    rStream.Write(SinAngle);
    rStream.Write(Exponent);

    SE_END_DEBUG_STREAM_SAVE(SELight);
}
//----------------------------------------------------------------------------
int SELight::GetDiskUsed(const SEStreamVersion& rVersion) const
{
    return SEAdvancedObject::GetDiskUsed(rVersion) +
        sizeof(Type) +
        sizeof(Position) +
        sizeof(RVector) +
        sizeof(UVector) +
        sizeof(DVector) +
        sizeof(Ambient) +
        sizeof(Diffuse) +
        sizeof(Specular) +
        sizeof(Intensity) +
        sizeof(Constant) +
        sizeof(Linear) +
        sizeof(Quadratic) +
        sizeof(Angle) +
        sizeof(CosAngle) +
        sizeof(SinAngle) +
        sizeof(Exponent);
}
//----------------------------------------------------------------------------