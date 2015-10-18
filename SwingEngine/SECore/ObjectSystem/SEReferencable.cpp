//----------------------------------------------------------------------------
// Graphics framework for real-time GI study.
// Che Sun at Worcester Polytechnic Institute, Fall 2013.
//----------------------------------------------------------------------------
#include "SECorePCH.h"
#include "SEReferencable.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEReferencable::SEReferencable()
{
	mReferenceCount = 0;
}
//----------------------------------------------------------------------------
SEReferencable::~SEReferencable()
{
}
//----------------------------------------------------------------------------
void SEReferencable::DecrementReferenceCount()
{
    if( SE_AtomicAdd(&mReferenceCount, -1) == 0 )
    {
        SE_DELETE this;
    }
}
//----------------------------------------------------------------------------