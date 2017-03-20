// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERenderingEnginePCH.h"
#include "SETechnique.h"

using namespace Swing;

//----------------------------------------------------------------------------
SETechnique::SETechnique()
{
}
//----------------------------------------------------------------------------
SETechnique::SETechnique(SERenderPass* _pPass)
{
	AddPass(_pPass);
}
//----------------------------------------------------------------------------
SETechnique::SETechnique(std::initializer_list<SERenderPass*> _passes)
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