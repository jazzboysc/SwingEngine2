// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_BossaNovaHeaders_H
#define Swing_BossaNovaHeaders_H

// Bossa Nova Register
#include "SEBossaNovaRegister.h"

// Spectrum
#include "SESpectrumUtility.h"
#include "SECoefficientSpectrum.h"
#include "SESampledSpectrum.h"
#include "SERGBSpectrum.h"
#include "SESpectrum.h"

// Rendering
#include "SEBNRenderer.h"
#include "SESamplerRenderer.h"

// Scene
#include "SEBNScene.h"
#include "SEDifferentialGeometry.h"
#include "SEBNShape.h"
#include "SEBNTriangleMesh.h"
#include "SEBNTriangle.h"
#include "SEBNPrimitive.h"
#include "SEBNIntersection.h"
#include "SEBNGeometricPrimitive.h"
#include "SEBNInstancedPrimitive.h"
#include "SEBNAggregate.h"
#include "SEBNKdTree.h"
#include "SEBNVolumeRegion.h"

// BSDFs
#include "SEBSDF.h"
#include "SEBSSRDF.h"
#include "SEBxDF.h"
#include "SEBSDFUtility.h"
#include "SEBRDFToBTDF.h"
#include "SEScaledBxDF.h"
#include "SEFresnel.h"
#include "SEFresnelConductor.h"
#include "SEFresnelDielectric.h"
#include "SESpecularReflection.h"
#include "SESpecularTransmission.h"
#include "SELambertian.h"
#include "SEBSDFSample.h"

// Materials
#include "SEBNMaterial.h"
#include "SEBNMatteMaterial.h"

// Cameras
#include "SEFilm.h"
#include "SEImageFilm.h"
#include "SEBNCamera.h"
#include "SEBNProjectiveCamera.h"
#include "SEBNPerspectiveCamera.h"

// Samplers
#include "SEBNSampler.h"
#include "SEBNStratifiedSampler.h"

// Shaders
#include "SEBNShader.h"
#include "SEBNSurfaceShader.h"
#include "SEBNVolumeShader.h"
#include "SEBNDirectLightingShader.h"
#include "SEBNPathTracingShader.h"
#include "SEBNPhotonMapShader.h"

// Filters
#include "SEFilter.h"
#include "SEBoxFilter.h"
#include "SEGaussianFilter.h"
#include "SEMitchellFilter.h"
#include "SETriangleFilter.h"

// Textures
#include "SEBNTexture.h"
#include "SEBNConstantTexture.h"

// Lights
#include "SEBNLightSample.h"
#include "SEVisibilityTester.h"
#include "SEBNLight.h"
#include "SEBNShapeSet.h"
#include "SEBNDiffuseAreaLight.h"

#endif