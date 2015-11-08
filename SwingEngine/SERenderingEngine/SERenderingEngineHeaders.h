// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_RenderingEngineHeaders_H
#define Swing_RenderingEngineHeaders_H

// Rendering Engine Register
#include "SERenderingEngineRegister.h"

// Buffers
#include "SEBufferView.h"
#include "SEBufferBase.h"
#include "SEBuffer.h"
#include "SEIndexableBuffer.h"
#include "SEPixelBuffer.h"
#include "SEAtomicCounterBuffer.h"
#include "SETextureBuffer.h"
#include "SEStructuredBuffer.h"
#include "SEDispatchIndirectBuffer.h"
#include "SEUniformBuffer.h"
#include "SEVertexBuffer.h"
#include "SEIndexBuffer.h"

// Textures
#include "SETexture.h"
#include "SETexture1D.h"
#include "SETexture2D.h"
#include "SETexture3D.h"
#include "SETextureCube.h"
#include "SETexture2DArray.h"
#include "SEBufferTexture.h"
#include "SEFrameBuffer.h"
#include "SETextureSampler.h"
#include "SETextureManager.h"

// GPU Device
#include "SEGPUDevice.h"
#include "SEGPUResource.h"
#include "SEGPUDeviceInspector.h"
#include "SEGPUDeviceResident.h"

// Thin GPU Device
#include "SEThinGPUDevice.h"
#include "SECommandAllocator.h"
#include "SECommandList.h"
#include "SECommandQueue.h"
#include "SEDescriptor.h"
#include "SEDescriptorHeap.h"
#include "SEThinGPUDeviceFence.h"

// GPGPU
#include "SEComputePass.h"
#include "SEComputeTask.h"

// Renderers
#include "SERendererInput.h"
#include "SERendererOutput.h"
#include "SESubRenderer.h"
#include "SEGBufferRenderer.h"
#include "SEShadowMapRenderer.h"
#include "SEVoxelizer.h"
#include "SEPipelineStateBlock.h"

// Scene Graph
#include "SEBoundingVolume.h"
#include "SEBoxBV.h"
#include "SECamera.h"
#include "SECameraNode.h"
#include "SECuller.h"
#include "SELight.h"
#include "SELightNode.h"
#include "SENode.h"
#include "SESpatial.h"
#include "SESphereBV.h"
#include "SEUnculledObject.h"
#include "SEUnculledSet.h"

// Scene Objects
#include "SERTGICamera.h"
#include "SEFog.h"
#include "SERTGILight.h"
#include "SELightManager.h"
#include "SEPointSet.h"
#include "SERenderObject.h"
#include "SETriangleMesh.h"
#include "SEScreenQuad.h"
#include "SEGeometryAttributes.h"
#include "SERenderSet.h"
#include "SEQuadMesh.h"
#include "SERenderCache.h"
#include "SESpatialInfo.h"
#include "SEPrimitive.h"
#include "SERenderSequence.h"

// Shader Floats
#include "SEShaderFloat.h"

// Shaders
#include "SEShader.h"
#include "SEVertexShader.h"
#include "SEGeometryShader.h"
#include "SEFragmentShader.h"
#include "SEComputeShader.h"
#include "SETessellationControlShader.h"
#include "SETessellationEvaluationShader.h"
#include "SEShaderProgram.h"
#include "SEPassBase.h"
#include "SEPass.h"
#include "SEPassInfo.h"
#include "SEPassManager.h"
#include "SETechnique.h"
#include "SETechniqueInfo.h"
#include "SEMaterialTemplate.h"
#include "SEShaderUniform.h"

// Utils
#include "SEGPUTimer.h"
#include "SETerminal.h"

#endif