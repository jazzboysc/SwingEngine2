// Swing Engine Version 2 Source Code 
// Most of techniques in the engine are mainly based on David Eberly's
// Wild Magic 4 and 5 open-source game engine. The author of Swing Engine 
// learned a lot from Eberly's experience of architecture and algorithm.
// Several subsystems are totally new, and others are reorganized or
// reimplimented based on Wild Magic's subsystems.
// Copyright (c) 2007-2011
//
// David Eberly's permission:
// Geometric Tools, LLC
// Copyright (c) 1998-2010
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt

#ifndef Swing_ShaderFloat_H
#define Swing_ShaderFloat_H

#include "SERenderingEngineLIB.h"
#include "SEObject.h"

namespace Swing
{

class SECamera;
class SEGeometry;

//----------------------------------------------------------------------------
// Description:
// Date:20100629
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEShaderFloat : public SEObject
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    SEShaderFloat(void);
    SEShaderFloat(int iDataCount);
    virtual ~SEShaderFloat(void);

    // For deferred construction.
    void SetDataCount(int iDataCount);

    // Member access.
    inline int GetDataCount(void) const;
    inline const float* GetData(void) const;
    inline float* GetData(void);

    // Access to data.
    // SetData function copies m_iDataCount floats from 'pSrcData' into 
    // 'm_afData'. Similar float-sizes are copied by the GetData function.
    void SetData(const float* pSrcData);
    void GetData(float* pDstData);

    // The same as SetData. This is useful when the source data is
    // an object that defines an implict conversion to 'const float*'.
    SEShaderFloat& operator = (const float* pSrcData);

    // Access to data by index.
    inline float operator [] (int i) const;
    inline float& operator [] (int i);

    // This function allows for dynamic updates of the shader constants
    // in the SEEffectInput::Enable function that the renderer calls.
    // Thus, the transfer of the data to the shader programs occurs as
    // late as possible in the rendering pass. This is useful for global
    // effects that modify camera settings during their drawing calls.
    inline void EnableUpdater(void);
    inline void DisableUpdater(void);
    inline bool AllowUpdater(void) const;
    virtual void Update(const SEGeometry* pGeometry, 
        const SECamera* pCamera);

protected:
    int m_iDataCount;
    float* m_afData;
    bool m_bAllowUpdater;
};

typedef SESmartPointer<SEShaderFloat> SEShaderFloatPtr;

#include "SEShaderFloat.inl"

}

#endif
