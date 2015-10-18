// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SEComputeTask.h"
#include "SEComputePass.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEComputeTask::SEComputeTask()
{
}
//----------------------------------------------------------------------------
SEComputeTask::~SEComputeTask()
{
}
//----------------------------------------------------------------------------
void SEComputeTask::CreateDeviceResource(SEGPUDevice* device)
{
    SEPassManager::CreateDeviceResource(device);
    OnGetShaderConstants();
}
//----------------------------------------------------------------------------
void SEComputeTask::DispatchCompute(unsigned int pass, unsigned int globalX,
    unsigned int globalY, unsigned int globalZ)
{
    SEComputePass* p = (SEComputePass*)GetPass(pass);
    SE_ASSERT(p);
    SE_ASSERT(p->IsVertexPass() == false);

    SEShaderProgram* program = p->GetShaderProgram();
    program->Enable();
    OnPreDispatch(pass);

    program->GetProgramHandle()->Device->ComputeShaderDispatch(
		program, globalX, globalY, globalZ);

    OnPostDispatch(pass);
    program->Disable();
}
//----------------------------------------------------------------------------
void SEComputeTask::DispatchComputeIndirect(unsigned int pass,
    SEBuffer* indirectCommandBuffer, SEBufferView* indirectCommandBufferView, 
    void* indirect)
{
	SEComputePass* p = (SEComputePass*)GetPass(pass);
	SE_ASSERT(p);
	SE_ASSERT(indirectCommandBuffer && indirectCommandBufferView);

	SEShaderProgram* program = p->GetShaderProgram();
	program->Enable();
	OnPreDispatch(pass);

	indirectCommandBuffer->BindTo(indirectCommandBufferView);
	program->GetProgramHandle()->Device->ComputeShaderDispatchIndirect(indirect);

	OnPostDispatch(pass);
	program->Disable();
}
//----------------------------------------------------------------------------
void SEComputeTask::DispatchVertex(unsigned int pass, unsigned int threadCount)
{
    SEComputePass* p = (SEComputePass*)GetPass(pass);
    SE_ASSERT(p);
    SE_ASSERT(p->IsVertexPass() == true);

    SEShaderProgram* program = p->GetShaderProgram();
    program->Enable();
    OnPreDispatch(pass);

    program->GetProgramHandle()->Device->DispatchVertex(threadCount);

    OnPostDispatch(pass);
    program->Disable();
}
//----------------------------------------------------------------------------
void SEComputeTask::DispatchVertexIndirect(unsigned int pass,
    SEBuffer* indirectCommandBuffer, SEBufferView* indirectCommandBufferView, 
    void* indirect)
{
    SEComputePass* p = (SEComputePass*)GetPass(pass);
    SE_ASSERT(p);
    SE_ASSERT(p->IsVertexPass() == true);
    SE_ASSERT(indirectCommandBuffer && indirectCommandBufferView);

    SEShaderProgram* program = p->GetShaderProgram();
    program->Enable();
    OnPreDispatch(pass);

    indirectCommandBuffer->BindTo(indirectCommandBufferView);
    program->GetProgramHandle()->Device->DispatchVertexIndirect(indirect);

    OnPostDispatch(pass);
    program->Disable();
}
//----------------------------------------------------------------------------