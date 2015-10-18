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
#include "SEController.h"
#include "SEAdvancedObject.h"
#include "SEMath.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SEController, SEObject);
SE_IMPLEMENT_ABSTRACT_STREAM(SEController);
SE_IMPLEMENT_DEFAULT_NAME_ID(SEController, SEObject);

const char* SEController::ms_aacRepeatType[SEController::RT_COUNT] =
{
    "RT_CLAMP",
    "RT_WRAP",
    "RT_CYCLE"
};

//----------------------------------------------------------------------------
SEController::SEController()
{
    mObject = 0;
    Repeat = RT_CLAMP;
    MinTime = 0.0;
    MaxTime = 0.0;
    Phase = 0.0;
    Frequency = 1.0;
    Active = true;
    m_dLastAppTime = -DBL_MAX;
}
//----------------------------------------------------------------------------
SEController::~SEController()
{
}
//----------------------------------------------------------------------------
double SEController::GetControlTime(double dAppTime)
{
    double dCtrlTime = Frequency*dAppTime + Phase;

    if( Repeat == RT_CLAMP )
    {
        if( dCtrlTime < MinTime )
        {
            return MinTime;
        }
        if( dCtrlTime > MaxTime )
        {
            return MaxTime;
        }

        return dCtrlTime;
    }

    double dRange = MaxTime - MinTime;
    if( dRange > 0.0 )
    {
        double dMultiples = (dCtrlTime - MinTime)/dRange;
        double dIntTime = SEMathd::Floor(dMultiples);
        double dFrcTime = dMultiples - dIntTime;
        if( Repeat == RT_WRAP )
        {
            return MinTime + dFrcTime*dRange;
        }

        // Repeat == RT_CYCLE
        if( ((int)dIntTime) & 1 )
        {
            // backward time
            return MaxTime - dFrcTime*dRange;
        }
        else
        {
            // forward time
            return MinTime + dFrcTime*dRange;
        }
    }
    else
    {
        return MinTime;
    }
}
//----------------------------------------------------------------------------
bool SEController::Update(double dAppTime)
{
    if( Active &&  
        (dAppTime == -SEMathd::MAX_REAL || dAppTime != m_dLastAppTime) )
    {
        m_dLastAppTime = dAppTime;

        return true;
    }

    return false;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Streaming support.
//----------------------------------------------------------------------------
void SEController::Load(SEStream& rStream, SEStream::SELink* pLink)
{
    SE_BEGIN_DEBUG_STREAM_LOAD;

    SEObject::Load(rStream, pLink);

    // Native data.
    int iRepeat;
    rStream.Read(iRepeat);
    Repeat = (RepeatType)iRepeat;

    rStream.Read(MinTime);
    rStream.Read(MaxTime);
    rStream.Read(Phase);
    rStream.Read(Frequency);
    rStream.Read(Active);
    // m_dLastAppTime is not streamed

    // Link data.
    SEObject* pObject;
    rStream.Read(pObject);
    pLink->Add(pObject);

    SE_END_DEBUG_STREAM_LOAD(SEController);
}
//----------------------------------------------------------------------------
void SEController::Link(SEStream& rStream, SEStream::SELink* pLink)
{
    SEObject::Link(rStream, pLink);

    SEObject* pLinkID = pLink->GetLinkID();
    mObject = (SEAdvancedObject*)rStream.GetFromMap(pLinkID);
}
//----------------------------------------------------------------------------
bool SEController::Register(SEStream& rStream) const
{
    if( !SEObject::Register(rStream) )
    {
        return false;
    }

    if( mObject )
    {
        mObject->Register(rStream);
    }

    return true;
}
//----------------------------------------------------------------------------
void SEController::Save(SEStream& rStream) const
{
    SE_BEGIN_DEBUG_STREAM_SAVE;

    SEObject::Save(rStream);

    // Native data.
    rStream.Write((int)Repeat);
    rStream.Write(MinTime);
    rStream.Write(MaxTime);
    rStream.Write(Phase);
    rStream.Write(Frequency);
    rStream.Write(Active);

    // Link data.
    rStream.Write(mObject);

    SE_END_DEBUG_STREAM_SAVE(SEController);
}
//----------------------------------------------------------------------------
int SEController::GetDiskUsed(const SEStreamVersion& rVersion) const
{
    return SEObject::GetDiskUsed(rVersion) +
        sizeof(int) +  // Repeat
        sizeof(MinTime) +
        sizeof(MaxTime) +
        sizeof(Phase) +
        sizeof(Frequency) +
        sizeof(char) +  // Active
        SE_STREAM_PTRSIZE(mObject);
}
//----------------------------------------------------------------------------