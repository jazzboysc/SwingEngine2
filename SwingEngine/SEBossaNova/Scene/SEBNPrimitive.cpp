// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017
//
// This part of Swing Engine is based on PBRT.
/*
    pbrt source code Copyright(c) 1998-2012 Matt Pharr and Greg Humphreys.

    This file is part of pbrt.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

#include "SEBossaNovaPCH.h"
#include "SEBNPrimitive.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SEBNPrimitive, SEObject);

SE_UInt32 SEBNPrimitive::NextPrimitiveID = 1;

//----------------------------------------------------------------------------
SEBNPrimitive::SEBNPrimitive()
    :
    PrimitiveID(NextPrimitiveID++)
{
}
//----------------------------------------------------------------------------
SEBNPrimitive::~SEBNPrimitive()
{
}
//----------------------------------------------------------------------------
bool SEBNPrimitive::CanIntersect() const
{
    return true;
}
//----------------------------------------------------------------------------
void SEBNPrimitive::Refine(std::vector<SEBNPrimitivePtr>&) const
{
    // Unimplemented SEBNPrimitive::Refine() method called.
    SE_ASSERT( false );
}
//----------------------------------------------------------------------------
void SEBNPrimitive::FullyRefine(std::vector<SEBNPrimitivePtr>& refined) const
{
    std::vector<SEBNPrimitivePtr> todo;
    todo.push_back(const_cast<SEBNPrimitive*>(this));

    while( todo.size() )
    {
        // Refine last primitive in todo list.
        SEBNPrimitivePtr prim = todo.back();
        todo.pop_back();

        if( prim->CanIntersect() )
        {
            refined.push_back(prim);
        }
        else
        {
            prim->Refine(todo);
        }
    }
}
//----------------------------------------------------------------------------