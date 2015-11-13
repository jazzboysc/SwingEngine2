// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

//----------------------------------------------------------------------------
void SEGPUDeviceBase::Terminate()
{
    (this->*_Terminate)();
}
//----------------------------------------------------------------------------
void SEGPUDeviceBase::Initialize(SEGPUDeviceDescription* deviceDesc)
{
    mDeviceDesc = *deviceDesc;
    (this->*_Initialize)(deviceDesc);
}
//----------------------------------------------------------------------------
void SEGPUDeviceBase::SetAnisFilterLevel(int maxAnisFilterLevel)
{
    (this->*_SetAnisFilterLevel)(maxAnisFilterLevel);
}
//----------------------------------------------------------------------------
void SEGPUDeviceBase::GetMaxAnisFilterLevel(int* maxAnisFilterLevel)
{
    (this->*_GetMaxAnisFilterLevel)(maxAnisFilterLevel);
}
//----------------------------------------------------------------------------
SEShaderHandle* SEGPUDeviceBase::CreateShader(SEShader* shader)
{
    return (this->*_CreateShader)(shader);
}
//----------------------------------------------------------------------------
void SEGPUDeviceBase::DeleteShader(SEShader* shader)
{
    (this->*_DeleteShader)(shader);
}
//----------------------------------------------------------------------------
SEPassInfoHandle* SEGPUDeviceBase::CreatePassInfo(SEPassInfo* passInfo,
    SEShaderProgram* program, SEGeometryAttributes* geometryAttr,
    SEPipelineStateBlock* psb)
{
    return (this->*_CreatePassInfo)(passInfo, program, geometryAttr, psb);
}
//----------------------------------------------------------------------------
void SEGPUDeviceBase::DeletePassInfo(SEPassInfo* passInfo)
{
    (this->*_DeletePassInfo)(passInfo);
}
//----------------------------------------------------------------------------
void SEGPUDeviceBase::EnablePassInfo(SEPassInfo* passInfo)
{
    (this->*_EnablePassInfo)(passInfo);
}
//----------------------------------------------------------------------------
void SEGPUDeviceBase::DisablePassInfo(SEPassInfo* passInfo)
{
    (this->*_DisablePassInfo)(passInfo);
}
//----------------------------------------------------------------------------