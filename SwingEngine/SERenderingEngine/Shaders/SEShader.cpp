// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SEShader.h"
#include <fstream>

using namespace Swing;

//----------------------------------------------------------------------------
SEShader::SEShader(SEShaderType type, const std::string& shaderFileName)
	:
	mType(type),
	mShaderHandle(0)
{
    mShaderFileName = "Shaders/" + shaderFileName;
    bool res = LoadFromFile(mShaderFileName);
	if( !res )
	{
		SE_ASSERT(false);
	}

	// TODO:
	// Assign defalut source if LoadFromFile fails.
}
//----------------------------------------------------------------------------
SEShader::~SEShader()
{
    if( mShaderHandle )
    {
        mShaderHandle->Device->DeleteShader(this);
        SE_DELETE mShaderHandle;
        mShaderHandle = 0;
    }
}
//----------------------------------------------------------------------------
void SEShader::CreateDeviceResource(SEGPUDevice* device)
{
	if( mShaderHandle )
	{
		return;
	}

    mShaderHandle = device->CreateShader(this);
}
//----------------------------------------------------------------------------
SEShaderHandle* SEShader::GetShaderHandle() const
{
	return mShaderHandle;
}
//----------------------------------------------------------------------------
std::string SEShader::GetShaderFileName() const
{
    return mShaderFileName;
}
//----------------------------------------------------------------------------
SEShaderType SEShader::GetType() const
{
    return mType;
}
//----------------------------------------------------------------------------
const std::string& SEShader::GetSource() const
{
    return mShaderSource;
}
//----------------------------------------------------------------------------
bool SEShader::LoadFromFile(const std::string& shaderFileName)
{
	std::ifstream shaderFileData(shaderFileName);
    if( !shaderFileData )
    {
        return false;
    }
	
    size_t startPos, endPos, range;
	std::string curLine;
	while( !shaderFileData.eof() )
	{
		getline(shaderFileData, curLine);

        startPos = curLine.find("#include");
        if( startPos == std::string::npos )
        {
            mShaderSource += curLine + "\n";
        }
        else
        {
            startPos += 8;
            startPos = curLine.find_first_not_of(" \t", startPos);
            startPos += 1;
            endPos = curLine.find_first_of("\"", startPos);
            range = endPos - startPos;
            std::string includeFileName = "Shaders/" + curLine.substr(
                startPos, range);

            bool res = LoadFromFile(includeFileName);
			if( !res )
			{
				SE_ASSERT(false);
			}
        }
	}

	return true;
}
//----------------------------------------------------------------------------