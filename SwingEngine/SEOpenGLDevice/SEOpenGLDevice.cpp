// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SEOpenGLDevicePCH.h"
#include "SEOpenGLDevice.h"
#include "SEGPUDeviceInspector.h"
#include "SETerminal.h"
#include "SEShaderProgram.h"
#include "SEBufferView.h"
#include "SEBufferBase.h"
#include "SETexture.h"
#include "SETexture1D.h"
#include "SETexture2D.h"
#include "SETexture2DArray.h"
#include "SETexture3D.h"
#include "SEPixelBuffer.h"
#include "SEFrameBuffer.h"
#include "SEPrimitive.h"
#include "SEPassInfo.h"
#include "SEPipelineStateBlock.h"
#include "SEGPUTimer.h"

#include <string>
#include <GL/glew.h>

#define RTGI_OUTPUT_SHADER_RESOURCE_LOADING
#define RTGI_GPU_MEMORY_INSPECTION

using namespace Swing;

GLenum gsShaderType[ShaderType_Max] = 
{
    GL_VERTEX_SHADER,
    GL_FRAGMENT_SHADER,
    GL_GEOMETRY_SHADER,
    GL_COMPUTE_SHADER,
    GL_TESS_CONTROL_SHADER,
    GL_TESS_EVALUATION_SHADER,
};

GLenum gsShaderProgramParams[SPP_Max] = 
{
    GL_GEOMETRY_VERTICES_OUT_EXT
};

GLenum gsBufferTargets[BufferType_Max] =
{
    GL_ATOMIC_COUNTER_BUFFER,
    GL_DISPATCH_INDIRECT_BUFFER,
    GL_DRAW_INDIRECT_BUFFER,
    GL_PIXEL_UNPACK_BUFFER,
    GL_SHADER_STORAGE_BUFFER,
    GL_TEXTURE_BUFFER,
    GL_UNIFORM_BUFFER,
    GL_ARRAY_BUFFER,
    GL_ELEMENT_ARRAY_BUFFER
};

GLenum gsBufferBindings[BufferType_Max] =
{
    GL_ATOMIC_COUNTER_BUFFER_BINDING,
    GL_DISPATCH_INDIRECT_BUFFER_BINDING,
    GL_DRAW_INDIRECT_BUFFER_BINDING,
    GL_PIXEL_UNPACK_BUFFER_BINDING,
    GL_SHADER_STORAGE_BUFFER_BINDING,
    GL_TEXTURE_BUFFER_BINDING,
    GL_UNIFORM_BUFFER_BINDING,
    GL_ARRAY_BUFFER_BINDING,
    GL_ELEMENT_ARRAY_BUFFER_BINDING
};

GLenum gsBufferFormat[BufferFormat_Max] =
{
    GL_R,
    GL_RG,
    GL_RGB,
    GL_RGBA,
    GL_RGB,
    GL_RGBA,
	GL_RGBA,
    GL_RED_INTEGER,
    GL_RED,
    GL_DEPTH_COMPONENT
};

GLint gsBufferInternalFormat[BufferInternalFormat_Max] =
{
    GL_RGB8,
    GL_RGBA8,
    GL_RGB32F_ARB,
    GL_RGBA32F_ARB,
    GL_RGBA32UI,
    GL_RGB16F_ARB,
    GL_RGBA16F_ARB,
    GL_R32UI,
    GL_R32F,
    GL_RG32F,
    GL_DEPTH_COMPONENT24
};

GLenum gsBufferComponentType[BufferComponentType_Max] =
{
    GL_UNSIGNED_BYTE,
    GL_UNSIGNED_INT,
    GL_FLOAT,
	GL_HALF_FLOAT
};

GLenum gsBufferAccess[BufferAccess_Max] =
{
    GL_READ_ONLY,
    GL_WRITE_ONLY,
    GL_READ_WRITE
};

GLenum gsBufferUsage[BufferUsage_Max] =
{
    GL_STATIC_READ,
    GL_STATIC_COPY,
    GL_STATIC_DRAW,
    GL_DYNAMIC_READ,
    GL_DYNAMIC_COPY,
    GL_DYNAMIC_DRAW,
	GL_STREAM_READ,
	GL_STREAM_COPY,
	GL_STREAM_DRAW
};

GLenum gsTextureTargets[TextureType_Max] =
{
    GL_TEXTURE_1D,
    GL_TEXTURE_2D,
    GL_TEXTURE_3D,
    GL_TEXTURE_CUBE_MAP,
    GL_TEXTURE_2D_ARRAY,
    GL_TEXTURE_BUFFER
};

GLenum gsFilterType[FilterType_Max] =
{
    GL_NEAREST,
    GL_LINEAR,
    GL_NEAREST_MIPMAP_NEAREST,
    GL_LINEAR_MIPMAP_NEAREST,
    GL_NEAREST_MIPMAP_LINEAR,
    GL_LINEAR_MIPMAP_LINEAR
};

GLenum gsWrapType[WrapType_Max] =
{
    GL_CLAMP,
    GL_REPEAT
};

GLenum gsPrimitiveType[PrimitiveType_Max] = 
{
    GL_POINTS,
    GL_LINES,
    GL_TRIANGLES,
    GL_QUADS,
    GL_PATCHES,
};

#ifdef _DEBUG
//----------------------------------------------------------------------------
#define OPENGL_DEVICE_CHECK_ERROR  \
    {  \
        GLenum res = glGetError();  \
        SE_ASSERT(res == GL_NO_ERROR);  \
    }
//----------------------------------------------------------------------------
#else
#define OPENGL_DEVICE_CHECK_ERROR
#endif

#ifdef RTGI_GPU_MEMORY_INSPECTION
//----------------------------------------------------------------------------
#define OPENGL_DEVICE_BEGIN_MEM_INSPECTION(inspector, resident)  \
    int mem1 = 0;  \
    if( inspector )  \
    {  \
        mem1 = GetDeviceMemoryAvailable();  \
    }
//----------------------------------------------------------------------------
#define OPENGL_DEVICE_END_MEM_INSPECTION(inspector, resident)  \
    int mem2 = 0;  \
    if( inspector )  \
    {  \
        mem2 = GetDeviceMemoryAvailable();  \
        SEDeviceMemOp op;  \
        op.Resident = resident;  \
        op.DeviceMemPrint = mem1 - mem2;  \
        inspector->OnDeviceMemoryOperation(&op);  \
    }
//----------------------------------------------------------------------------
#else
#define OPENGL_DEVICE_BEGIN_MEM_INSPECTION(inspector, resident) (void)resident
#define OPENGL_DEVICE_END_MEM_INSPECTION(inspector, resident) (void)resident
#endif

#define INSERT_GPU_FUNC(x) _##x = (GPUDevice##x)&SEOpenGLDevice::__##x

#define GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX          0x9047
#define GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX    0x9048
#define GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX  0x9049
#define GPU_MEMORY_INFO_EVICTION_COUNT_NVX            0x904A
#define GPU_MEMORY_INFO_EVICTED_MEMORY_NVX            0x904B

//----------------------------------------------------------------------------
void SEOpenGLDevice::__Initialize(SEGPUDeviceDescription*)
{
    // TODO:
    //  Should create OpenGL context here. Nothing to do now since we are
    // using GLFW.

	// Set device info.
	// TODO:
	// Only support NVIDIA for now.
	const GLubyte* str = glGetString(GL_VENDOR);
	std::string vendor((char*)str);
	if( vendor.find("NVIDIA") != std::string::npos )
	{
		mDeviceInfo.Vendor = DV_NVIDIA;
		glGetIntegerv(GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, 
			(GLint*)&mDeviceInfo.GPUTotalMemory);
		mDeviceInfo.GPUTotalMemory = mDeviceInfo.GPUTotalMemory >> 10;
		glGetIntegerv(GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX,
			(GLint*)&mDeviceInfo.GPUDedicatedMemory);
		mDeviceInfo.GPUDedicatedMemory = mDeviceInfo.GPUDedicatedMemory >> 10;
		mDeviceInfo.Valid = true;
	}

    // Default global states.
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glFrontFace(GL_CW);
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__Terminate()
{
    // TODO:
    //  Should destroy OpenGL context here. Nothing to do now since we are
    // using GLFW.
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__OnResize(unsigned int, unsigned int)
{
    // TODO:
}
//----------------------------------------------------------------------------
SEShaderHandle* SEOpenGLDevice::__CreateShader(SEShader* shader)
{
    SEOpenGLShaderHandle* shaderHandle = SE_NEW SEOpenGLShaderHandle();
    shaderHandle->Device = this;

    const char* programText = shader->GetSource().c_str();
    GLenum shaderType = gsShaderType[(int)shader->GetType()];
    const std::string fileName = shader->GetShaderFileName();

    shaderHandle->mShader = glCreateShader(shaderType);
    glShaderSource(shaderHandle->mShader, 1, &programText, 0);
    glCompileShader(shaderHandle->mShader);

    GLint compiled;
    glGetShaderiv(shaderHandle->mShader, GL_COMPILE_STATUS, &compiled);
    if( !compiled )
    {
        GLint iInfoLen = 0;
        glGetShaderiv(shaderHandle->mShader, GL_INFO_LOG_LENGTH, &iInfoLen);
        if( iInfoLen > 1 )
        {
            char* acInfoLog = SE_NEW char[iInfoLen];
            glGetShaderInfoLog(shaderHandle->mShader, iInfoLen, 0, acInfoLog);
            SETerminal::Output(SETerminal::OC_Error, "Failed compiling %s\n%s\n", 
                fileName.c_str(), acInfoLog);

            SE_DELETE[] acInfoLog;
        }

        SE_ASSERT(false);
        SE_DELETE  shaderHandle;
        return 0;
    }

#ifdef RTGI_OUTPUT_SHADER_RESOURCE_LOADING
    SETerminal::Output(SETerminal::OC_Success, "Loading shader %s finished\n", 
        fileName.c_str());
#endif

    OPENGL_DEVICE_CHECK_ERROR;

    return shaderHandle;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__DeleteShader(SEShader* shader)
{
    SEOpenGLShaderHandle* shaderHandle = 
        (SEOpenGLShaderHandle*)shader->GetShaderHandle();
    glDeleteShader(shaderHandle->mShader);

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
SEShaderProgramHandle* SEOpenGLDevice::__CreateProgram(SEShaderProgram* program)
{
    GLuint __program = glCreateProgram();
    std::string linkingString("Linking program");

    SEVertexShader* vShader = program->GetVertexShader();
    if( vShader )
    {
        vShader->CreateDeviceResource(this);
        SEOpenGLShaderHandle* vsHandle = 
            (SEOpenGLShaderHandle*)vShader->GetShaderHandle();
        glAttachShader(__program, vsHandle->mShader);
        linkingString += " ";
        linkingString += vShader->GetShaderFileName();
    }

    SEFragmentShader* fShader = program->GetFragmentShader();
    if( fShader )
    {
        fShader->CreateDeviceResource(this);
        SEOpenGLShaderHandle* fsHandle =
            (SEOpenGLShaderHandle*)fShader->GetShaderHandle();
        glAttachShader(__program, fsHandle->mShader);
        linkingString += " ";
        linkingString += fShader->GetShaderFileName();
    }

    SEGeometryShader* gShader = program->GetGeometryShader();
    if( gShader )
    {
        gShader->CreateDeviceResource(this);
        SEOpenGLShaderHandle* gsHandle =
            (SEOpenGLShaderHandle*)gShader->GetShaderHandle();
        glAttachShader(__program, gsHandle->mShader);
        linkingString += " ";
        linkingString += gShader->GetShaderFileName();
    }

    SEComputeShader* cShader = program->GetComputeShader();
    if( cShader )
    {
        cShader->CreateDeviceResource(this);
        SEOpenGLShaderHandle* csHandle =
            (SEOpenGLShaderHandle*)cShader->GetShaderHandle();
        glAttachShader(__program, csHandle->mShader);
        linkingString += " ";
        linkingString += cShader->GetShaderFileName();
    }

    SETessellationControlShader* tcShader = program->GetTessellationControlShader();
    if( tcShader )
    {
        tcShader->CreateDeviceResource(this);
        SEOpenGLShaderHandle* tcsHandle =
            (SEOpenGLShaderHandle*)tcShader->GetShaderHandle();
        glAttachShader(__program, tcsHandle->mShader);
        linkingString += " ";
        linkingString += tcShader->GetShaderFileName();
    }

    SETessellationEvaluationShader* teShader = program->GetTessellationEvaluationShader();
    if( teShader )
    {
        teShader->CreateDeviceResource(this);
        SEOpenGLShaderHandle* tesHandle =
            (SEOpenGLShaderHandle*)teShader->GetShaderHandle();
        glAttachShader(__program, tesHandle->mShader);
        linkingString += " ";
        linkingString += teShader->GetShaderFileName();
    }

    glLinkProgram(__program);
    GLint linked;
    glGetProgramiv(__program, GL_LINK_STATUS, &linked);
    if( !linked )
    {
        GLint iInfoLen = 0;
        glGetProgramiv(__program, GL_INFO_LOG_LENGTH, &iInfoLen);
        if( iInfoLen > 1 )
        {
            char* acInfoLog = SE_NEW char[iInfoLen];
            glGetProgramInfoLog(__program, iInfoLen, 0, acInfoLog);
            linkingString += " failed";

            SETerminal::Output(SETerminal::OC_Error, "%s\n%s\n", 
                linkingString.c_str(), acInfoLog);
            SE_DELETE[] acInfoLog;
        }

        SE_ASSERT(false);
        return 0;
    }

#ifdef RTGI_OUTPUT_SHADER_RESOURCE_LOADING
    linkingString += " finished";
    SETerminal::Output(SETerminal::OC_Success, "%s\n", linkingString.c_str());
#endif

    OPENGL_DEVICE_CHECK_ERROR;

    SEOpenGLShaderProgramHandle* programHandle = SE_NEW SEOpenGLShaderProgramHandle();
    programHandle->Device = this;
    programHandle->mProgram = __program;
    return programHandle;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__DeleteProgram(SEShaderProgram* program)
{
    SEOpenGLShaderProgramHandle* programHandle =
        (SEOpenGLShaderProgramHandle*)program->GetProgramHandle();
    glDeleteProgram(programHandle->mProgram);

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__EnableProgram(SEShaderProgram* program)
{
    SEOpenGLShaderProgramHandle* programHandle =
        (SEOpenGLShaderProgramHandle*)program->GetProgramHandle();
    glUseProgram(programHandle->mProgram);

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__DisableProgram(SEShaderProgram*)
{
    glUseProgram(0);

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__SetProgramParameterInt(SEShaderProgram* program, 
    SEShaderProgramParameter pname, int value)
{
    GLenum name = gsShaderProgramParams[(int)pname];
    SEOpenGLShaderProgramHandle* programHandle =
        (SEOpenGLShaderProgramHandle*)program->GetProgramHandle();
    glProgramParameteri(programHandle->mProgram, name, value);

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
SEPassInfoHandle* SEOpenGLDevice::__CreatePassInfo(SEPassInfo*, 
    SEShaderProgram* program, SEPrimitive* primitive, bool hasNormal, 
    bool hasTCoord, int vertexComponentCount)
{
    SEOpenGLPassInfoHandle* passInfoHandle = SE_NEW SEOpenGLPassInfoHandle();
    passInfoHandle->Device = this;

    SEOpenGLShaderProgramHandle* programHandle =
        (SEOpenGLShaderProgramHandle*)program->GetProgramHandle();
    SE_ASSERT(programHandle);
    
    glGenVertexArrays(1, &passInfoHandle->mVAO);
    glBindVertexArray(passInfoHandle->mVAO);
    
    if( primitive->IB )
    {
        primitive->IB->Bind();
    }
    if( primitive->VB )
    {
        primitive->VB->Bind();
    }
    
    // Specify vertex attributes.
    if( !hasNormal && !hasTCoord )
    {
        GLint loc = glGetAttribLocation(programHandle->mProgram, "vPosition");
        if( loc != -1 )
        {
            glEnableVertexAttribArray(loc);
            glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
        }
    }
    else if( hasNormal && !hasTCoord )
    {
        GLint loc = glGetAttribLocation(programHandle->mProgram, "vPosition");
        if( loc != -1 )
        {
            glEnableVertexAttribArray(loc);
            glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE,
                vertexComponentCount*sizeof(float), 0);
        }
    
        loc = glGetAttribLocation(programHandle->mProgram, "vNormal");
        if( loc != -1 )
        {
            glEnableVertexAttribArray(loc);
            glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE,
                vertexComponentCount*sizeof(float), (void*)12);
        }
    }
    else if( hasNormal && hasTCoord )
    {
        GLint loc = glGetAttribLocation(programHandle->mProgram, "vPosition");
        if( loc != -1 )
        {
            glEnableVertexAttribArray(loc);
            glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE,
                vertexComponentCount*sizeof(float), 0);
        }
    
        loc = glGetAttribLocation(programHandle->mProgram, "vTCoord");
        if( loc != -1 )
        {
            glEnableVertexAttribArray(loc);
            glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE,
                vertexComponentCount*sizeof(float), (void*)12);
        }
    
        loc = glGetAttribLocation(programHandle->mProgram, "vNormal");
        if( loc != -1 )
        {
            glEnableVertexAttribArray(loc);
            glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE,
                vertexComponentCount*sizeof(float), (void*)20);
        }
    }
    else
    {
        GLint loc = glGetAttribLocation(programHandle->mProgram, "vPosition");
        if( loc != -1 )
        {
            glEnableVertexAttribArray(loc);
            glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE,
                vertexComponentCount*sizeof(float), 0);
        }
    
        loc = glGetAttribLocation(programHandle->mProgram, "vTCoord");
        if( loc != -1 )
        {
            glEnableVertexAttribArray(loc);
            glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE,
                vertexComponentCount*sizeof(float), (void*)12);
        }
    }
    
    glBindVertexArray(0);
    
    OPENGL_DEVICE_CHECK_ERROR;

    return passInfoHandle;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__DeletePassInfo(SEPassInfo* passInfo)
{
    SEOpenGLPassInfoHandle* passInfoHandle = 
        (SEOpenGLPassInfoHandle*)passInfo->GetPassInfoHandle();
    glDeleteVertexArrays(1, &passInfoHandle->mVAO);

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__EnablePassInfo(SEPassInfo* passInfo)
{
    SEOpenGLPassInfoHandle* passInfoHandle =
        (SEOpenGLPassInfoHandle*)passInfo->GetPassInfoHandle();
    glBindVertexArray(passInfoHandle->mVAO);

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__DisablePassInfo(SEPassInfo*)
{
    glBindVertexArray(0);

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__GetUniformLocation(SEShaderProgram* program, 
    SEShaderUniform* uniform, const char* name)
{
    SEOpenGLShaderProgramHandle* programHandle = 
        (SEOpenGLShaderProgramHandle*)program->GetProgramHandle();
    SE_ASSERT(programHandle);

    GLint loc = glGetUniformLocation(programHandle->mProgram, name);
    if( !uniform->mUniformHandle )
    {
        uniform->mUniformHandle = SE_NEW SEOpenGLShaderUniformHandle();
        uniform->mUniformHandle->Device = this;
    }
    ((SEOpenGLShaderUniformHandle*)uniform->mUniformHandle)->mUniform = loc;

#ifdef _DEBUG
    if( loc < 0 )
    {
        SETerminal::Output(SETerminal::OC_Warning_Level4, 
            "Uniform: \'%s\' not found in: \n", name);
        SEVertexShader* vs = program->GetVertexShader();
        if( vs )
        {
            SETerminal::Output(SETerminal::OC_Warning_Level4, "    %s\n",
                vs->GetShaderFileName().c_str());
        }
        SEFragmentShader* fs = program->GetFragmentShader();
        if( fs )
        {
            SETerminal::Output(SETerminal::OC_Warning_Level4, "    %s\n",
                fs->GetShaderFileName().c_str());
        }
        SETessellationControlShader* tcs = program->GetTessellationControlShader();
        if( tcs )
        {
            SETerminal::Output(SETerminal::OC_Warning_Level4, "    %s\n",
                tcs->GetShaderFileName().c_str());
        }
        SETessellationEvaluationShader* tes = program->GetTessellationEvaluationShader();
        if( tes )
        {
            SETerminal::Output(SETerminal::OC_Warning_Level4, "    %s\n",
                tes->GetShaderFileName().c_str());
        }
        SEGeometryShader* gs = program->GetGeometryShader();
        if( gs )
        {
            SETerminal::Output(SETerminal::OC_Warning_Level4, "    %s\n",
                gs->GetShaderFileName().c_str());
        }
        SEComputeShader* cs = program->GetComputeShader();
        if( cs )
        {
            SETerminal::Output(SETerminal::OC_Warning_Level4, "    %s\n",
                cs->GetShaderFileName().c_str());
        }
    }
#endif

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__SetUniformValueMat4(SEShaderUniform* uniform, 
    const SEMatrix4f* value)
{
    SEOpenGLShaderUniformHandle* uniformHandle =
        (SEOpenGLShaderUniformHandle*)uniform->mUniformHandle;
    SE_ASSERT(uniformHandle);

    // TODO:
    // Fix this.
    glUniformMatrix4fv(uniformHandle->mUniform, 1, GL_FALSE, (GLfloat*)&(*value)[0][0]);

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__SetUniformValueVec3(SEShaderUniform* uniform, 
    const SEVector3f* value)
{
    SEOpenGLShaderUniformHandle* uniformHandle =
        (SEOpenGLShaderUniformHandle*)uniform->mUniformHandle;
    SE_ASSERT(uniformHandle);

    glUniform3fv(uniformHandle->mUniform, 1, (GLfloat*)value);

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__SetUniformValueVec4(SEShaderUniform* uniform,
	const SEVector4f* value)
{
	SEOpenGLShaderUniformHandle* uniformHandle =
		(SEOpenGLShaderUniformHandle*)uniform->mUniformHandle;
	SE_ASSERT(uniformHandle);

	glUniform4fv(uniformHandle->mUniform, 1, (GLfloat*)value);

	OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__SetUniformValueInt(SEShaderUniform* uniform, int value)
{
    SEOpenGLShaderUniformHandle* uniformHandle =
        (SEOpenGLShaderUniformHandle*)uniform->mUniformHandle;
    SE_ASSERT(uniformHandle);

    glUniform1i(uniformHandle->mUniform, value);

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__SetUniformValueUInt(SEShaderUniform* uniform, 
    unsigned int value)
{
    SEOpenGLShaderUniformHandle* uniformHandle =
        (SEOpenGLShaderUniformHandle*)uniform->mUniformHandle;
    SE_ASSERT(uniformHandle);

    glUniform1ui(uniformHandle->mUniform, value);

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__SetUniformValueFloat(SEShaderUniform* uniform, float value)
{
    SEOpenGLShaderUniformHandle* uniformHandle =
        (SEOpenGLShaderUniformHandle*)uniform->mUniformHandle;
    SE_ASSERT(uniformHandle);

    glUniform1f(uniformHandle->mUniform, value);

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__SetUniformValueFloat2(SEShaderUniform* uniform, 
    const float* value)
{
    SEOpenGLShaderUniformHandle* uniformHandle =
        (SEOpenGLShaderUniformHandle*)uniform->mUniformHandle;
    SE_ASSERT(uniformHandle);

    glUniform2fv(uniformHandle->mUniform, 1, value);

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__DeleteTexture(SETexture* texture)
{
    SEOpenGLTextureHandle* textureHandle = 
        (SEOpenGLTextureHandle*)texture->GetTextureHandle();
    if( textureHandle )
    {
        glDeleteTextures(1, &textureHandle->mTexture);
    }

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
SETextureHandle* SEOpenGLDevice::__Texture1DLoadFromSystemMemory(SETexture* texture,
    SEBufferInternalFormat internalFormat, int width, SEBufferFormat format, 
    SEBufferComponentType type, void* pixels)
{
    OPENGL_DEVICE_BEGIN_MEM_INSPECTION(mInspector, texture);

    SEOpenGLTextureHandle* textureHandle = SE_NEW SEOpenGLTextureHandle();
    textureHandle->Device = this;

    glGenTextures(1, &textureHandle->mTexture);
    glBindTexture(GL_TEXTURE_1D, textureHandle->mTexture);
    glTexImage1D(GL_TEXTURE_1D, 0, 
        gsBufferInternalFormat[(int)internalFormat], width, 0, 
        gsBufferFormat[(int)format], gsBufferComponentType[(int)type],
        pixels);

    glBindTexture(GL_TEXTURE_1D, 0);

    OPENGL_DEVICE_CHECK_ERROR;
    OPENGL_DEVICE_END_MEM_INSPECTION(mInspector, texture);

    return textureHandle;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__Texture1DUpdateFromPixelBuffer(SETexture* texture, 
    SEPixelBuffer* pixelBuffer)
{
    SEOpenGLTextureHandle* textureHandle = 
        (SEOpenGLTextureHandle*)texture->GetTextureHandle();
    if( !textureHandle )
    {
        SE_ASSERT(false);
        return;
    }

    GLuint buffer = 
        ((SEOpenGLBufferHandle*)pixelBuffer->GetBufferHandle())->mBuffer;
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, buffer);
    glBindTexture(GL_TEXTURE_1D, textureHandle->mTexture);
    glTexImage1D(GL_TEXTURE_1D, 0, 
        gsBufferInternalFormat[(int)texture->GetInternalFormat()], 
        ((SETexture1D*)texture)->Width, 0, 
        gsBufferFormat[(int)texture->GetFormat()],
        gsBufferComponentType[(int)texture->GetComponentType()],
    	0);
    glBindTexture(GL_TEXTURE_1D, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    
    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__TextureBindToImageUnit(SETexture* texture, 
	unsigned int unit, bool layered, SEBufferAccess access)
{
#if defined(__APPLE__)
    SE_ASSERT( false );
#else
    SEOpenGLTextureHandle* textureHandle =
        (SEOpenGLTextureHandle*)texture->GetTextureHandle();
    if( !textureHandle )
    {
        SE_ASSERT(false);
        return;
    }

	glBindImageTexture(unit, textureHandle->mTexture, 0, layered, 0,
        gsBufferAccess[(int)access],
        gsBufferInternalFormat[(int)texture->GetInternalFormat()]);
    
    OPENGL_DEVICE_CHECK_ERROR;
    
#endif
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__TextureBindToSampler(SETexture* texture, 
    unsigned int index, SESamplerDesc* sampler)
{
    SEOpenGLTextureHandle* textureHandle = 
        (SEOpenGLTextureHandle*)texture->GetTextureHandle();
    if( !textureHandle )
    {
        SE_ASSERT(false);
        return;
    }

    // Sampler binding.
    GLuint t = textureHandle->mTexture;
    SETextureType type = texture->GetType();
    GLenum target = gsTextureTargets[(int)type];
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(target, t);

    OPENGL_DEVICE_CHECK_ERROR;

	// Anisotropic Filtering.
	if( mAnisFilterLevel )
	{
		glTexParameteri(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, 
            mAnisFilterLevel);
	}

    OPENGL_DEVICE_CHECK_ERROR;

    if( sampler )
    {
        // Filtering.
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER,
            gsFilterType[(int)sampler->MinFilter]);

        OPENGL_DEVICE_CHECK_ERROR;

        glTexParameteri(target, GL_TEXTURE_MAG_FILTER,
            gsFilterType[(int)sampler->MagFilter]);

        OPENGL_DEVICE_CHECK_ERROR;

        // SETexture coordinates wrapping.
        glTexParameteri(target, GL_TEXTURE_WRAP_S,
            gsWrapType[(int)sampler->WrapS]);

        OPENGL_DEVICE_CHECK_ERROR;

        glTexParameteri(target, GL_TEXTURE_WRAP_T,
            gsWrapType[(int)sampler->WrapT]);

        OPENGL_DEVICE_CHECK_ERROR;

        if( target == GL_TEXTURE_CUBE_MAP )
        {
            glTexParameteri(target, GL_TEXTURE_WRAP_R,
                gsWrapType[(int)sampler->WrapR]);

            OPENGL_DEVICE_CHECK_ERROR;
        }
    }
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__TextureGenerateMipmap(SETexture* texture)
{
    SEOpenGLTextureHandle* textureHandle =
        (SEOpenGLTextureHandle*)texture->GetTextureHandle();
    SE_ASSERT(textureHandle);

    SETextureType type = texture->GetType();
    GLenum target = gsTextureTargets[(int)type];
    glBindTexture(target, textureHandle->mTexture);
    glGenerateMipmap(target);
    glBindTexture(target, 0);

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__Texture1DGetDataFromGPUMemory(SETexture* texture, 
    void* dstData)
{
    SEOpenGLTextureHandle* textureHandle =
        (SEOpenGLTextureHandle*)texture->GetTextureHandle();
    if( !textureHandle )
    {
        SE_ASSERT(false);
        return;
    }

    glBindTexture(GL_TEXTURE_1D, textureHandle->mTexture);
    glGetTexImage(GL_TEXTURE_1D, 0, 
        gsBufferFormat[(int)texture->GetFormat()],
        gsBufferComponentType[(int)texture->GetComponentType()],
        dstData);
    glBindTexture(GL_TEXTURE_1D, 0);
    
    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
SETextureHandle* SEOpenGLDevice::__Texture2DLoadFromSystemMemory(SETexture* texture,
    SEBufferInternalFormat internalFormat, int width, int height,
    SEBufferFormat format, SEBufferComponentType type, bool mipMap, 
    SESamplerDesc* sampler, void* pixels)
{
    OPENGL_DEVICE_BEGIN_MEM_INSPECTION(mInspector, texture);

    SEOpenGLTextureHandle* textureHandle = SE_NEW SEOpenGLTextureHandle();
    textureHandle->Device = this;

    glGenTextures(1, &textureHandle->mTexture);
    glBindTexture(GL_TEXTURE_2D, textureHandle->mTexture);
    glTexImage2D(GL_TEXTURE_2D, 0,
        gsBufferInternalFormat[(int)internalFormat], width, height, 0,
        gsBufferFormat[(int)format], gsBufferComponentType[(int)type],
        pixels);

    if( mipMap )
    {
        if( sampler )
        {
            // Mipmap filtering.
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                gsFilterType[(int)sampler->MinFilter]);

            OPENGL_DEVICE_CHECK_ERROR;

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                gsFilterType[(int)sampler->MagFilter]);

            OPENGL_DEVICE_CHECK_ERROR;

            // SETexture coordinates wrapping.
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                gsWrapType[(int)sampler->WrapS]);

            OPENGL_DEVICE_CHECK_ERROR;

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                gsWrapType[(int)sampler->WrapT]);

            OPENGL_DEVICE_CHECK_ERROR;
        }

        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    OPENGL_DEVICE_CHECK_ERROR;
    OPENGL_DEVICE_END_MEM_INSPECTION(mInspector, texture);

    return textureHandle;
}
//----------------------------------------------------------------------------
SETextureHandle* SEOpenGLDevice::__Texture2DLoadFromTextureBuffer(
    SETexture* texture, SETextureBuffer* textureBuffer, 
    SEBufferInternalFormat internalFormat)
{
    OPENGL_DEVICE_BEGIN_MEM_INSPECTION(mInspector, texture);

    SEOpenGLTextureHandle* textureHandle = SE_NEW SEOpenGLTextureHandle();
    textureHandle->Device = this;

    GLuint buffer = 
        ((SEOpenGLBufferHandle*)textureBuffer->GetBufferHandle())->mBuffer;
    glGenTextures(1, &textureHandle->mTexture);
    glBindTexture(GL_TEXTURE_BUFFER, textureHandle->mTexture);
    glTexBuffer(GL_TEXTURE_BUFFER, 
        gsBufferInternalFormat[(int)internalFormat], buffer);
    glBindTexture(GL_TEXTURE_BUFFER, 0);

    OPENGL_DEVICE_CHECK_ERROR;
    OPENGL_DEVICE_END_MEM_INSPECTION(mInspector, texture);

    return textureHandle;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__Texture2DUpdateFromPixelBuffer(SETexture* texture, 
    SEPixelBuffer* pixelBuffer)
{
    SEOpenGLTextureHandle* textureHandle = 
        (SEOpenGLTextureHandle*)texture->GetTextureHandle();
    SE_ASSERT(textureHandle);

    GLuint buffer = 
        ((SEOpenGLBufferHandle*)pixelBuffer->GetBufferHandle())->mBuffer;
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, buffer);
    glBindTexture(GL_TEXTURE_2D, textureHandle->mTexture);
    glTexImage2D(GL_TEXTURE_2D, 0,
        gsBufferInternalFormat[(int)texture->GetInternalFormat()], 
        ((SETexture2D*)texture)->Width, ((SETexture2D*)texture)->Height, 0,
        gsBufferFormat[(int)texture->GetFormat()],
        gsBufferComponentType[(int)texture->GetComponentType()], 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    
    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__Texture2DGetImageData(SETexture* texture, void* dstPixels)
{
    SEOpenGLTextureHandle* textureHandle =
        (SEOpenGLTextureHandle*)texture->GetTextureHandle();
    SE_ASSERT(textureHandle);

    glBindTexture(GL_TEXTURE_2D, textureHandle->mTexture);
    glGetTexImage(GL_TEXTURE_2D, 0, gsBufferFormat[(int)texture->GetFormat()],
        gsBufferComponentType[(int)texture->GetComponentType()], dstPixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
SETextureHandle* SEOpenGLDevice::__Tex2DArrayLoadFromSystemMemory(
    SETexture* texture, SEBufferInternalFormat internalFormat, int width, 
    int height, int depth, SEBufferFormat format, SEBufferComponentType type, 
	bool mipmap, void* pixels)
{
    OPENGL_DEVICE_BEGIN_MEM_INSPECTION(mInspector, texture);

    SEOpenGLTextureHandle* textureHandle = SE_NEW SEOpenGLTextureHandle();
    textureHandle->Device = this;

    glGenTextures(1, &textureHandle->mTexture);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureHandle->mTexture);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, 
        gsBufferInternalFormat[(int)internalFormat], width, height, depth,
        0, gsBufferFormat[(int)format], gsBufferComponentType[(int)type], 
        pixels);

	if (mipmap)
	{
		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	}

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    OPENGL_DEVICE_CHECK_ERROR;
    OPENGL_DEVICE_END_MEM_INSPECTION(mInspector, texture);

    return textureHandle;
}
//----------------------------------------------------------------------------
SETextureHandle* SEOpenGLDevice::__Texture3DLoadFromSystemMemory(
    SETexture* texture, SEBufferInternalFormat internalFormat, int width, 
    int height, int depth, SEBufferFormat format, SEBufferComponentType type, 
    void* pixels)
{
    OPENGL_DEVICE_BEGIN_MEM_INSPECTION(mInspector, texture);

    SEOpenGLTextureHandle* textureHandle = SE_NEW SEOpenGLTextureHandle();
    textureHandle->Device = this;

    glGenTextures(1, &textureHandle->mTexture);
    glBindTexture(GL_TEXTURE_3D, textureHandle->mTexture);
    glTexImage3D(GL_TEXTURE_3D, 0, 
        gsBufferInternalFormat[(int)internalFormat], width, height, depth, 0,
        gsBufferFormat[(int)format], gsBufferComponentType[(int)type], 
        pixels);

    glBindTexture(GL_TEXTURE_3D, 0);

    OPENGL_DEVICE_CHECK_ERROR;
    OPENGL_DEVICE_END_MEM_INSPECTION(mInspector, texture);

    return textureHandle;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__Texture3DUpdateFromPixelBuffer(SETexture* texture,
    SEPixelBuffer* pixelBuffer)
{
    SEOpenGLTextureHandle* textureHandle =
        (SEOpenGLTextureHandle*)texture->GetTextureHandle();
    SE_ASSERT(textureHandle);

    GLuint buffer = 
        ((SEOpenGLBufferHandle*)pixelBuffer->GetBufferHandle())->mBuffer;
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, buffer);
    glBindTexture(GL_TEXTURE_3D, textureHandle->mTexture);
    glTexImage3D(GL_TEXTURE_3D, 0,
        gsBufferInternalFormat[(int)texture->GetInternalFormat()],
        ((SETexture3D*)texture)->Width, ((SETexture3D*)texture)->Height,
        ((SETexture3D*)texture)->Depth, 0,
        gsBufferFormat[(int)texture->GetFormat()],
        gsBufferComponentType[(int)texture->GetComponentType()], 0);

    glBindTexture(GL_TEXTURE_3D, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    
    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
SETextureHandle* SEOpenGLDevice::__TextureCubeLoadFromSystemMemory(
    SETexture* texture, SEBufferInternalFormat internalFormat, int width, 
    int height, SEBufferFormat format, SEBufferComponentType type, bool mipMap, 
    void* pixelsPX, void* pixelsNX, void* pixelsPY, void* pixelsNY, 
    void* pixelsPZ, void* pixelsNZ)
{
    OPENGL_DEVICE_BEGIN_MEM_INSPECTION(mInspector, texture);

    SEOpenGLTextureHandle* textureHandle = SE_NEW SEOpenGLTextureHandle();
    textureHandle->Device = this;

    GLint tempInternalFormat = gsBufferInternalFormat[(int)internalFormat];
    GLenum tempFormat = gsBufferFormat[(int)format];
    GLenum tempType = gsBufferComponentType[(int)type];

    glGenTextures(1, &textureHandle->mTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureHandle->mTexture);
    
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, tempInternalFormat, width,
        height, 0, tempFormat, tempType, pixelsPX);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, tempInternalFormat, width,
        height, 0, tempFormat, tempType, pixelsNX);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, tempInternalFormat, width,
        height, 0, tempFormat, tempType, pixelsPY);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, tempInternalFormat, width,
        height, 0, tempFormat, tempType, pixelsNY);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, tempInternalFormat, width,
        height, 0, tempFormat, tempType, pixelsPZ);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, tempInternalFormat, width,
        height, 0, tempFormat, tempType, pixelsNZ);

    if( mipMap )
    {
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    
    OPENGL_DEVICE_CHECK_ERROR;
    OPENGL_DEVICE_END_MEM_INSPECTION(mInspector, texture);

    return textureHandle;
}
//----------------------------------------------------------------------------
SETextureHandle* SEOpenGLDevice::__BufferTextureLoadFromTextureBuffer(
    SETexture* texture, SETextureBuffer* textureBuffer, 
    SEBufferInternalFormat internalFormat)
{
    OPENGL_DEVICE_BEGIN_MEM_INSPECTION(mInspector, texture);

    SEOpenGLTextureHandle* textureHandle = SE_NEW SEOpenGLTextureHandle();
    textureHandle->Device = this;

    GLuint buffer =
        ((SEOpenGLBufferHandle*)textureBuffer->GetBufferHandle())->mBuffer;
    glGenTextures(1, &textureHandle->mTexture);
    glBindTexture(GL_TEXTURE_BUFFER, textureHandle->mTexture);
    glTexBuffer(GL_TEXTURE_BUFFER, 
        gsBufferInternalFormat[(int)internalFormat], buffer);
    glBindTexture(GL_TEXTURE_BUFFER, 0);

    OPENGL_DEVICE_CHECK_ERROR;
    OPENGL_DEVICE_END_MEM_INSPECTION(mInspector, texture);

    return textureHandle;
}
//----------------------------------------------------------------------------
SEFBOHandle* SEOpenGLDevice::__CreateFrameBuffer(SEFrameBuffer* frameBuffer)
{
    OPENGL_DEVICE_BEGIN_MEM_INSPECTION(mInspector, frameBuffer);

    SEOpenGLFBOHandle* fboHandle = SE_NEW SEOpenGLFBOHandle();
    fboHandle->Device = this;

    glGenFramebuffersEXT(1, &fboHandle->mFBO);

    OPENGL_DEVICE_CHECK_ERROR;
    OPENGL_DEVICE_END_MEM_INSPECTION(mInspector, frameBuffer);

    return fboHandle;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__DeleteFrameBuffer(SEFrameBuffer* frameBuffer)
{
    SEOpenGLFBOHandle* fboHandle = 
        (SEOpenGLFBOHandle*)frameBuffer->GetFBOHandle();
    SE_ASSERT(fboHandle);

    glDeleteFramebuffersEXT(1, &fboHandle->mFBO);
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__FrameBufferSetRenderTargets(SEFrameBuffer* frameBuffer,
    unsigned int colorTextureCount, SETexture** colorTextures, 
    SETexture* depthTexture, SETexture* stencilTexture)
{
    OPENGL_DEVICE_BEGIN_MEM_INSPECTION(mInspector, frameBuffer);

    SEOpenGLFBOHandle* fboHandle =
        (SEOpenGLFBOHandle*)frameBuffer->GetFBOHandle();
    SE_ASSERT(fboHandle);

    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle->mFBO);

    for( unsigned int i = 0; i < colorTextureCount; ++i )
    {
        SEOpenGLTextureHandle* textureHandle = 
            (SEOpenGLTextureHandle*)colorTextures[i]->GetTextureHandle();
        SE_ASSERT(textureHandle);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
            textureHandle->mTexture, 0);
        frameBuffer->mColorBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
        frameBuffer->mColorTextures.push_back(colorTextures[i]);
    }

    OPENGL_DEVICE_CHECK_ERROR;

    if( depthTexture )
    {
        SEOpenGLTextureHandle* textureHandle = 
            (SEOpenGLTextureHandle*)depthTexture->GetTextureHandle();
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
            textureHandle->mTexture, 0);
    }
    frameBuffer->mDepthTexture = depthTexture;

    OPENGL_DEVICE_CHECK_ERROR;

    if( stencilTexture )
    {
        SEOpenGLTextureHandle* textureHandle = 
            (SEOpenGLTextureHandle*)stencilTexture->GetTextureHandle();
        glFramebufferTexture(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, 
            textureHandle->mTexture, 0);
    }
    frameBuffer->mStencilTexture = stencilTexture;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    OPENGL_DEVICE_CHECK_ERROR;
    OPENGL_DEVICE_END_MEM_INSPECTION(mInspector, frameBuffer);
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__FrameBufferSetColorTarget(SEFrameBuffer* frameBuffer, 
    unsigned int attachPoint, SETexture* colorTexture)
{
    OPENGL_DEVICE_BEGIN_MEM_INSPECTION(mInspector, frameBuffer);

    SEOpenGLFBOHandle* fboHandle =
        (SEOpenGLFBOHandle*)frameBuffer->GetFBOHandle();
    SE_ASSERT(fboHandle);

    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle->mFBO);

    if( colorTexture )
    {
        SEOpenGLTextureHandle* textureHandle =
            (SEOpenGLTextureHandle*)colorTexture->GetTextureHandle();
        SE_ASSERT(textureHandle);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachPoint,
            textureHandle->mTexture, 0);
    }
    else
    {
        glFramebufferTexture(GL_FRAMEBUFFER, 
            GL_COLOR_ATTACHMENT0 + attachPoint, 0, 0);
    }
    frameBuffer->mColorBuffers[attachPoint] =
        GL_COLOR_ATTACHMENT0 + attachPoint;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    OPENGL_DEVICE_CHECK_ERROR;
    OPENGL_DEVICE_END_MEM_INSPECTION(mInspector, frameBuffer);
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__FrameBufferSetDepthTarget(SEFrameBuffer* frameBuffer, 
    SETexture* depthTexture)
{
    OPENGL_DEVICE_BEGIN_MEM_INSPECTION(mInspector, frameBuffer);

    SEOpenGLFBOHandle* fboHandle =
        (SEOpenGLFBOHandle*)frameBuffer->GetFBOHandle();
    SE_ASSERT(fboHandle);

    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle->mFBO);

    if( depthTexture )
    {
        SEOpenGLTextureHandle* textureHandle =
            (SEOpenGLTextureHandle*)depthTexture->GetTextureHandle();
        SE_ASSERT(textureHandle);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
            textureHandle->mTexture, 0);
    }
    else
    {
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    OPENGL_DEVICE_CHECK_ERROR;
    OPENGL_DEVICE_END_MEM_INSPECTION(mInspector, frameBuffer);
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__FrameBufferSetStencilTarget(SEFrameBuffer* frameBuffer, 
    SETexture* stencilTexture)
{
    OPENGL_DEVICE_BEGIN_MEM_INSPECTION(mInspector, frameBuffer);

    SEOpenGLFBOHandle* fboHandle =
        (SEOpenGLFBOHandle*)frameBuffer->GetFBOHandle();
    SE_ASSERT(fboHandle);

    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle->mFBO);

    if( stencilTexture )
    {
        SEOpenGLTextureHandle* textureHandle =
            (SEOpenGLTextureHandle*)stencilTexture->GetTextureHandle();
        SE_ASSERT(textureHandle);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
            textureHandle->mTexture, 0);
    }
    else
    {
        glFramebufferTexture(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, 0, 0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    OPENGL_DEVICE_CHECK_ERROR;
    OPENGL_DEVICE_END_MEM_INSPECTION(mInspector, frameBuffer);
}
//----------------------------------------------------------------------------
static GLint oldViewport[4];
//----------------------------------------------------------------------------
void SEOpenGLDevice::__FrameBufferEnable(SEFrameBuffer* frameBuffer)
{
    // Cache old viewport values and set new values.
    glGetIntegerv(GL_VIEWPORT, oldViewport);
    glViewport(0, 0, frameBuffer->mWidth, frameBuffer->mHeight);

    SEOpenGLFBOHandle* fboHandle =
        (SEOpenGLFBOHandle*)frameBuffer->GetFBOHandle();
    SE_ASSERT(fboHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle->mFBO);
    glDrawBuffers(frameBuffer->mColorTextureCount, frameBuffer->mColorBuffers);

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__FrameBufferDisable(SEFrameBuffer*)
{
    glViewport(oldViewport[0], oldViewport[1], oldViewport[2], 
        oldViewport[3]);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__ComputeShaderDispatch(SEShaderProgram*, 
    unsigned int globalX, unsigned int globalY, unsigned int globalZ)
{
    glDispatchCompute(globalX, globalY, globalZ);

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__ComputeShaderDispatchIndirect(void* indirect)
{
	glDispatchComputeIndirect((GLintptr)indirect);

	OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__DispatchVertex(unsigned int threadCount)
{
    glDrawArrays(GL_POINTS, 0, threadCount);

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__DispatchVertexIndirect(void* indirect)
{
    glDrawArraysIndirect(GL_POINTS, indirect);

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__DeleteBuffer(SEBuffer* buffer)
{
    SEOpenGLBufferHandle* bufferHandle = 
        (SEOpenGLBufferHandle*)buffer->GetBufferHandle();
    SE_ASSERT(bufferHandle);
    glDeleteBuffers(1, &bufferHandle->mBuffer);

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void* SEOpenGLDevice::__BufferMap(SEBuffer* buffer, SEBufferAccess access)
{
#ifdef _DEBUG
    SEOpenGLBufferHandle* bufferHandle =
        (SEOpenGLBufferHandle*)buffer->GetBufferHandle();
    SE_ASSERT(bufferHandle);

    GLint curBufferBound;
    glGetIntegerv(gsBufferBindings[(int)buffer->GetType()], &curBufferBound);
    SE_ASSERT(bufferHandle->mBuffer == (GLuint)curBufferBound);
#endif

    void* data = glMapBuffer(gsBufferTargets[(int)buffer->GetType()], 
        gsBufferAccess[(int)access]);

    OPENGL_DEVICE_CHECK_ERROR;

    return data;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__BufferUnmap(SEBuffer* buffer)
{
    glUnmapBuffer(gsBufferTargets[(int)buffer->GetType()]);

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__BufferBindIndex(SEBuffer* buffer, unsigned int index)
{
#ifndef __APPLE__

    GLuint b = ((SEOpenGLBufferHandle*)buffer->GetBufferHandle())->mBuffer;
    glBindBufferBase(gsBufferTargets[(int)buffer->GetType()], index, b);
    
    OPENGL_DEVICE_CHECK_ERROR;

#else
    SE_ASSERT(false);
#endif
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__BufferBindIndexTo(SEBuffer* buffer, unsigned int index, 
    SEBufferView* view)
{
#ifndef __APPLE__

    GLuint b = ((SEOpenGLBufferHandle*)buffer->GetBufferHandle())->mBuffer;
    glBindBufferBase(gsBufferTargets[(int)view->GetBufferType()], index, b);

    OPENGL_DEVICE_CHECK_ERROR;

#else
    SE_ASSERT(false);
#endif
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__BufferBind(SEBuffer* buffer)
{
    GLuint b = ((SEOpenGLBufferHandle*)buffer->GetBufferHandle())->mBuffer;
    GLenum target = gsBufferTargets[(int)buffer->GetType()];
    glBindBuffer(target, b);

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__BufferBindTo(SEBuffer* buffer, SEBufferView* view)
{
    GLuint b = ((SEOpenGLBufferHandle*)buffer->GetBufferHandle())->mBuffer;
    glBindBuffer(gsBufferTargets[(int)view->GetBufferType()], b);

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
SEBufferViewHandle* SEOpenGLDevice::__CreateBufferView(SEBufferView*)
{
    SEOpenGLBufferViewHandle* bufferViewHandle = SE_NEW SEOpenGLBufferViewHandle();
    bufferViewHandle->Device = this;
    return bufferViewHandle;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__DeleteBufferView(SEBufferView*)
{
    // Nothing to do.
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__BufferUpdateSubData(SEBuffer* buffer, int offset, 
    size_t size, void* data)
{
    glBufferSubData(gsBufferTargets[(int)buffer->GetType()], offset, size, 
        data);

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
SEBufferHandle* SEOpenGLDevice::__BufferLoadFromSystemMemory(SEBuffer* buffer, 
    size_t size, void* data, SEBufferUsage usage)
{
    OPENGL_DEVICE_BEGIN_MEM_INSPECTION(mInspector, buffer);

    SEOpenGLBufferHandle* bufferHandle = SE_NEW SEOpenGLBufferHandle();
    bufferHandle->Device = this;

    GLenum type = gsBufferTargets[(int)buffer->GetType()];
    glGenBuffers(1, &bufferHandle->mBuffer);
    glBindBuffer(type, bufferHandle->mBuffer);
    glBufferData(type, size, data, gsBufferUsage[(int)usage]);
    glBindBuffer(type, 0);

    OPENGL_DEVICE_CHECK_ERROR;
    OPENGL_DEVICE_END_MEM_INSPECTION(mInspector, buffer);

    return bufferHandle;
}
//----------------------------------------------------------------------------
SEBufferHandle* SEOpenGLDevice::__BufferLoadImmutableFromSystemMemory(
    SEBuffer* buffer, size_t size, void* data)
{
    OPENGL_DEVICE_BEGIN_MEM_INSPECTION(mInspector, buffer);

    SEOpenGLBufferHandle* bufferHandle = SE_NEW SEOpenGLBufferHandle();
    bufferHandle->Device = this;

    GLenum type = gsBufferTargets[(int)buffer->GetType()];
    glGenBuffers(1, &bufferHandle->mBuffer);
    glBindBuffer(type, bufferHandle->mBuffer);
    glBufferStorage(type, size, data, 0);
    glBindBuffer(type, 0);

    OPENGL_DEVICE_CHECK_ERROR;
    OPENGL_DEVICE_END_MEM_INSPECTION(mInspector, buffer);

    return bufferHandle;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__BufferClear(SEBuffer* buffer, 
    SEBufferInternalFormat internalFormat, SEBufferFormat format, 
    SEBufferComponentType type, void* data)
{
    GLuint b = ((SEOpenGLBufferHandle*)buffer->GetBufferHandle())->mBuffer;

    glInvalidateBufferData(b);
    glClearBufferData(gsBufferTargets[(int)buffer->GetType()], 
        gsBufferInternalFormat[(int)internalFormat],
        gsBufferFormat[(int)format], gsBufferComponentType[(int)type], data);

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
int SEOpenGLDevice::__GetDeviceMemoryAvailable()
{
	// TODO:
	// Only support NVIDIA for now.
	SE_ASSERT(mDeviceInfo.Valid && mDeviceInfo.Vendor == DV_NVIDIA);

	GLint res;
	glGetIntegerv(GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &res);
	return res >> 10;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__MemoryBarrier(unsigned int flags)
{
    GLbitfield barriers​ = 0;

    if( flags & MBT_ALL )
    {
        barriers​ = GL_ALL_BARRIER_BITS;
    }
    else
    {
        if( flags & MBT_Command )
        {
            barriers​ |= GL_COMMAND_BARRIER_BIT;
        }
        if( flags & MBT_AtomicCounter )
        {
            barriers​ |= GL_ATOMIC_COUNTER_BARRIER_BIT;
        }
        if( flags & MBT_Structured )
        {
            barriers​ |= GL_SHADER_STORAGE_BARRIER_BIT;
        }
    }
    glMemoryBarrier(barriers​);

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__ApplyPipelineStateBlock(SEPipelineStateBlock* psb)
{
    // Rasterizer states.
    if( psb->PipelineStageFlag & PB_Rasterizer )
    {
        // Viewport.
        if( psb->Rasterizer.RasterizerOpFlag & RB_Viewport )
        {
            glViewport(psb->Rasterizer.Viewport.X,
                psb->Rasterizer.Viewport.Y, psb->Rasterizer.Viewport.Width,
                psb->Rasterizer.Viewport.Height);
        }

        // Polygon fill mode.
        if( psb->Rasterizer.RasterizerOpFlag & RB_FillMode )
        {
            if( psb->Rasterizer.FillMode == PFM_Solid )
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
        }

        // Polygon cull mode.
        if( psb->Rasterizer.RasterizerOpFlag & RB_CullMode )
        {
            if( psb->Rasterizer.CullMode == PCM_Cull_None )
            {
                glDisable(GL_CULL_FACE);
            }
            else
            {
                glEnable(GL_CULL_FACE);

                if( psb->Rasterizer.CullMode == PCM_Cull_Front )
                {
                    glCullFace(GL_FRONT);
                }

                if( psb->Rasterizer.CullMode == PCM_Cull_Back )
                {
                    glCullFace(GL_BACK);
                }
            }
        }

        // Polygon front face.
        if( psb->Rasterizer.RasterizerOpFlag & RB_FrontFace )
        {
            if( psb->Rasterizer.FrontFaceClockwise )
            {
                glFrontFace(GL_CW);
            }
            else
            {
                glFrontFace(GL_CCW);
            }
        }
    }

    // Output merger states.
    if( psb->PipelineStageFlag & PB_OutputMerger )
    {
        if( psb->OutputMerger.OutputMergerOpFlag & OMB_Clear )
        {
            GLbitfield mask = 0;
            mask |= psb->OutputMerger.ClearMask & OMCB_Color_Buffer ? GL_COLOR_BUFFER_BIT : 0;
            mask |= psb->OutputMerger.ClearMask & OMCB_Depth_Buffer ? GL_DEPTH_BUFFER_BIT : 0;
            mask |= psb->OutputMerger.ClearMask & OMCB_Stencil_Buffer ? GL_STENCIL_BUFFER_BIT : 0;
            glClear(mask);
        }

        if( psb->OutputMerger.OutputMergerOpFlag & OMB_ClearColor )
        {
            glClearColor(psb->OutputMerger.ClearColor.X, psb->OutputMerger.ClearColor.Y,
                psb->OutputMerger.ClearColor.Z, psb->OutputMerger.ClearColor.W);
        }

        if( psb->OutputMerger.OutputMergerOpFlag & OMB_DepthStencil )
        {
            if( psb->OutputMerger.DepthStencil.DepthEnable )
            {
                glEnable(GL_DEPTH_TEST);
            }
            else
            {
                glDisable(GL_DEPTH_TEST);
            }

            if( psb->OutputMerger.DepthStencil.StencilEnable )
            {
                glEnable(GL_STENCIL_TEST);
            }
            else
            {
                glDisable(GL_STENCIL_TEST);
            }
        }

        if( psb->OutputMerger.OutputMergerOpFlag & OMB_Blend )
        {
            if( psb->OutputMerger.Blend.BlendStateOpFlag & BSB_ColorMask )
            {
                // TODO:
                // Only support target 0 for now.
                SE_UInt8& colorMask = psb->OutputMerger.Blend.FrameBufferTarget[0].ColorMask;
                GLboolean r = !(colorMask & CMB_Red);
                GLboolean g = !(colorMask & CMB_Green);
                GLboolean b = !(colorMask & CMB_Blue);
                GLboolean a = !(colorMask & CMB_Alpha);
                glColorMask(r, g, b, a);
            }
        }
    }

    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__SetPatchVertices(int value)
{
    glPatchParameteri(GL_PATCH_VERTICES, value);
    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__DrawPrimitive(SEPrimitiveType type, int count)
{
    SE_ASSERT( type != PT_Unknown );

    glDrawArrays(gsPrimitiveType[(int)type], 0, count);
    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__DrawPrimitiveInstanced(SEPrimitiveType type, int count,
    int instanceCount)
{
    SE_ASSERT( type != PT_Unknown );

    glDrawArraysInstanced(gsPrimitiveType[(int)type], 0, count, instanceCount);
    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__DrawIndexedPrimitive(SEPrimitiveType type, int count)
{
    SE_ASSERT( type != PT_Unknown );

    glDrawElements(gsPrimitiveType[(int)type], count, GL_UNSIGNED_INT, 0);
    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__DrawIndexedPrimitiveIndirect(SEPrimitiveType type,
    unsigned int commandOffset)
{
    SE_ASSERT( type != PT_Unknown );

    glDrawElementsIndirect(gsPrimitiveType[(int)type], GL_UNSIGNED_INT, 
        (void*)commandOffset);
    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__SetPointSize(float value)
{
    glPointSize(value);
    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__GetViewport(SEViewportState* dstViewport)
{
    SE_ASSERT( dstViewport );

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    dstViewport->X = viewport[0];
    dstViewport->Y = viewport[1];
    dstViewport->Width = viewport[2];
    dstViewport->Height = viewport[3];
    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__SetViewport(SEViewportState* srcViewport)
{
    SE_ASSERT(srcViewport);

    glViewport(srcViewport->X, srcViewport->Y, srcViewport->Width, 
        srcViewport->Height);
    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
SEGPUTimerHandle* SEOpenGLDevice::__CreateTimer(SEGPUTimer*)
{
    SEOpenGLTimerHandle* timerHandle = SE_NEW SEOpenGLTimerHandle();
    timerHandle->Device = this;

    glGenQueries(1, &timerHandle->mTimer);
    OPENGL_DEVICE_CHECK_ERROR;

    return timerHandle;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__DeleteTimer(SEGPUTimer* timer)
{
    SEOpenGLTimerHandle* timerHandle =
        (SEOpenGLTimerHandle*)timer->GetTimerHandle();
    glDeleteQueries(1, &timerHandle->mTimer);
    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__TimerStart(SEGPUTimer* timer)
{
    SEOpenGLTimerHandle* timerHandle =
        (SEOpenGLTimerHandle*)timer->GetTimerHandle();
    SE_ASSERT(timerHandle);

    glBeginQuery(GL_TIME_ELAPSED, timerHandle->mTimer);
    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__TimerStop(SEGPUTimer* timer)
{
    SEOpenGLTimerHandle* timerHandle =
        (SEOpenGLTimerHandle*)timer->GetTimerHandle();
    SE_ASSERT(timerHandle);

    glEndQuery(GL_TIME_ELAPSED);
    OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
double SEOpenGLDevice::__TimerGetTimeElapsed(SEGPUTimer* timer)
{
    SEOpenGLTimerHandle* timerHandle =
        (SEOpenGLTimerHandle*)timer->GetTimerHandle();
    SE_ASSERT(timerHandle);

    double res = 0.0;
    GLuint64 timeElapsed = 0;

    GLint available = 0;
    while( !available )
    {
        glGetQueryObjectiv(timerHandle->mTimer, GL_QUERY_RESULT_AVAILABLE, &available);
    }
    glGetQueryObjectui64v(timerHandle->mTimer, GL_QUERY_RESULT, &timeElapsed);

    res = double(timeElapsed) / 1000000.0;
    return res;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
SEOpenGLDevice::SEOpenGLDevice()
{    
    INSERT_GPU_FUNC(Initialize);
    INSERT_GPU_FUNC(Terminate);
    INSERT_GPU_FUNC(CreateShader);
    INSERT_GPU_FUNC(DeleteShader);
    INSERT_GPU_FUNC(CreateProgram);
    INSERT_GPU_FUNC(DeleteProgram);
    INSERT_GPU_FUNC(EnableProgram);
    INSERT_GPU_FUNC(DisableProgram);
    INSERT_GPU_FUNC(SetProgramParameterInt);
    INSERT_GPU_FUNC(CreatePassInfo);
    INSERT_GPU_FUNC(DeletePassInfo);
    INSERT_GPU_FUNC(EnablePassInfo);
    INSERT_GPU_FUNC(DisablePassInfo);
    INSERT_GPU_FUNC(GetUniformLocation);
    INSERT_GPU_FUNC(SetUniformValueMat4);
    INSERT_GPU_FUNC(SetUniformValueVec3);
    INSERT_GPU_FUNC(SetUniformValueVec4);
    INSERT_GPU_FUNC(SetUniformValueInt);
    INSERT_GPU_FUNC(SetUniformValueUInt);
    INSERT_GPU_FUNC(SetUniformValueFloat);
    INSERT_GPU_FUNC(SetUniformValueFloat2);
    INSERT_GPU_FUNC(DeleteTexture);
    INSERT_GPU_FUNC(Texture1DLoadFromSystemMemory);
    INSERT_GPU_FUNC(Texture1DUpdateFromPixelBuffer);
    INSERT_GPU_FUNC(TextureBindToImageUnit);
    INSERT_GPU_FUNC(TextureBindToSampler);
    INSERT_GPU_FUNC(TextureGenerateMipmap);
    INSERT_GPU_FUNC(Texture1DGetDataFromGPUMemory);
    INSERT_GPU_FUNC(Texture2DLoadFromSystemMemory);
    INSERT_GPU_FUNC(Texture2DLoadFromTextureBuffer);
    INSERT_GPU_FUNC(Texture2DUpdateFromPixelBuffer);
    INSERT_GPU_FUNC(Texture2DGetImageData);
    INSERT_GPU_FUNC(Tex2DArrayLoadFromSystemMemory);
    INSERT_GPU_FUNC(Texture3DLoadFromSystemMemory);
    INSERT_GPU_FUNC(Texture3DUpdateFromPixelBuffer);
    INSERT_GPU_FUNC(TextureCubeLoadFromSystemMemory);
    INSERT_GPU_FUNC(BufferTextureLoadFromTextureBuffer);
    INSERT_GPU_FUNC(CreateFrameBuffer);
    INSERT_GPU_FUNC(DeleteFrameBuffer);
    INSERT_GPU_FUNC(FrameBufferSetRenderTargets);
    INSERT_GPU_FUNC(FrameBufferSetColorTarget);
    INSERT_GPU_FUNC(FrameBufferSetDepthTarget);
    INSERT_GPU_FUNC(FrameBufferSetStencilTarget);
    INSERT_GPU_FUNC(FrameBufferEnable);
    INSERT_GPU_FUNC(FrameBufferDisable);
    INSERT_GPU_FUNC(ComputeShaderDispatch);
	INSERT_GPU_FUNC(ComputeShaderDispatchIndirect);
    INSERT_GPU_FUNC(DispatchVertex);
    INSERT_GPU_FUNC(DispatchVertexIndirect);
    INSERT_GPU_FUNC(DeleteBuffer);
    INSERT_GPU_FUNC(BufferMap);
    INSERT_GPU_FUNC(BufferUnmap);
    INSERT_GPU_FUNC(BufferBindIndex);
    INSERT_GPU_FUNC(BufferBindIndexTo);
    INSERT_GPU_FUNC(BufferBind);
    INSERT_GPU_FUNC(BufferBindTo);
    INSERT_GPU_FUNC(CreateBufferView);
    INSERT_GPU_FUNC(DeleteBufferView);
    INSERT_GPU_FUNC(BufferUpdateSubData);
    INSERT_GPU_FUNC(BufferLoadFromSystemMemory);
    INSERT_GPU_FUNC(BufferLoadImmutableFromSystemMemory);
    INSERT_GPU_FUNC(BufferClear);
    INSERT_GPU_FUNC(GetMaxAnisFilterLevel);
    INSERT_GPU_FUNC(SetAnisFilterLevel);
    INSERT_GPU_FUNC(GetDeviceMemoryAvailable);
    INSERT_GPU_FUNC(MemoryBarrier);
    INSERT_GPU_FUNC(ApplyPipelineStateBlock);
    INSERT_GPU_FUNC(SetPatchVertices);
    INSERT_GPU_FUNC(DrawPrimitive);
    INSERT_GPU_FUNC(DrawPrimitiveInstanced);
    INSERT_GPU_FUNC(DrawIndexedPrimitive);
    INSERT_GPU_FUNC(DrawIndexedPrimitiveIndirect);
    INSERT_GPU_FUNC(SetPatchVertices);
    INSERT_GPU_FUNC(SetPointSize);
    INSERT_GPU_FUNC(GetViewport);
    INSERT_GPU_FUNC(SetViewport);
    INSERT_GPU_FUNC(CreateTimer);
    INSERT_GPU_FUNC(DeleteTimer);
    INSERT_GPU_FUNC(TimerStart);
    INSERT_GPU_FUNC(TimerStop);
    INSERT_GPU_FUNC(TimerGetTimeElapsed);

    mEnable4xMsaa = false;
    m4xMsaaQuality = 0;
	mAnisFilterLevel = 0;
    mMaxTextureArrayLayer = 0;
}
//----------------------------------------------------------------------------
SEOpenGLDevice::~SEOpenGLDevice()
{
}
//----------------------------------------------------------------------------
/* From: https://www.opengl.org/archives/resources/features/OGLextensions/ */
int SEOpenGLDevice::__IsExtensionSupported(const char *extension)
{
	const GLubyte *extensions = NULL;
	const GLubyte *start;
	GLubyte *where, *terminator;

	/* Extension names should not have spaces. */
	where = (GLubyte *)strchr(extension, ' ');
	if (where || *extension == '\0')
		return 0;
	extensions = glGetString(GL_EXTENSIONS);
	/* It takes a bit of care to be fool-proof about parsing the
	OpenGL extensions string. Don't be fooled by sub-strings,
	etc. */
	start = extensions;
	for (;;)
	{
		where = (GLubyte *)strstr((const char *)start, extension);
		if (!where)
			break;
		terminator = where + strlen(extension);
		if (where == start || *(where - 1) == ' ')
		if (*terminator == ' ' || *terminator == '\0')
			return 1;
		start = terminator;
	}
	return 0;
}
//----------------------------------------------------------------------------

void SEOpenGLDevice::__GetMaxAnisFilterLevel(int* maxAnisFilterLevel)
{
	// TODO: maybe move this somewhere else
	if (__IsExtensionSupported("GL_EXT_texture_filter_anisotropic"))
	{
		glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisFilterLevel);
	}
	else
	{
		*maxAnisFilterLevel = 0;
	}
}
//----------------------------------------------------------------------------

void SEOpenGLDevice::__SetAnisFilterLevel(int anisFilterLevel)
{
	GLint maxAnisFilterLevel;
	__GetMaxAnisFilterLevel(&maxAnisFilterLevel);

	if (maxAnisFilterLevel >= anisFilterLevel)
	{
		mAnisFilterLevel = anisFilterLevel;
	}
}

//----------------------------------------------------------------------------