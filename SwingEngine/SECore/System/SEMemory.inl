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

//----------------------------------------------------------------------------
inline size_t& SEMemory::MaxAllowedBytes()
{
    return CurMaxAllowedBytes;
}
//----------------------------------------------------------------------------
inline bool& SEMemory::TrackSizes()
{
    return IsTrackSizes;
}
//----------------------------------------------------------------------------
inline size_t SEMemory::GetNumNewCalls()
{
    return CurNumNewCalls;
}
//----------------------------------------------------------------------------
inline size_t SEMemory::GetNumDeleteCalls()
{
    return CurNumDeleteCalls;
}
//----------------------------------------------------------------------------
inline size_t SEMemory::GetNumBlocks()
{
    return CurNumBlocks;
}
//----------------------------------------------------------------------------
inline size_t SEMemory::GetNumBytes()
{
    return CurNumBytes;
}
//----------------------------------------------------------------------------
inline size_t SEMemory::GetMaxAllocatedBytes()
{
    return CurMaxAllocatedBytes;
}
//----------------------------------------------------------------------------
inline size_t SEMemory::GetMaxBlockSize()
{
    return CurMaxBlockSize;
}
//----------------------------------------------------------------------------
inline size_t SEMemory::GetHistogram(int i)
{
    if( 0 <= i && i <= 31 )
    {
        return Histogram[i];
    }

    return 0;
}
//----------------------------------------------------------------------------
inline const SEMemory::SEMemoryBlock* SEMemory::GetHead()
{
    return MemBlockHead;
}
//----------------------------------------------------------------------------
inline const SEMemory::SEMemoryBlock* SEMemory::GetTail()
{
    return MemBlockTail;
}
//----------------------------------------------------------------------------
