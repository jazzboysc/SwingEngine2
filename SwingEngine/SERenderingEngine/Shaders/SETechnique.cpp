// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SETechnique.h"

using namespace Swing;

//----------------------------------------------------------------------------
SETechnique::SETechnique()
{
}
//----------------------------------------------------------------------------
SETechnique::SETechnique(SEPass* _pPass)
{
	AddPass(_pPass);
}
//----------------------------------------------------------------------------
SETechnique::SETechnique(std::initializer_list<SEPass*> _passes)
{
	for (auto i : _passes)
	{
		AddPass(i);
	}
}

//----------------------------------------------------------------------------
SETechnique::~SETechnique()
{
}
//----------------------------------------------------------------------------