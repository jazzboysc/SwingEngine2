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

#ifndef Swing_ColorRGBA_H
#define Swing_ColorRGBA_H

#include "SECoreLIB.h"
#include "SEPlatforms.h"
#include "SESystem.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20080312
//----------------------------------------------------------------------------
class SE_CORE_API SEColorRGBA
{
public:
    union
    {
        struct
        {
            float R;
            float G;
            float B;
            float A;     
        };
        float m_fData[4];
    };

public:
    // RGBAֵ��Χ��[0,1]֮��
    SEColorRGBA(void);  // ��ʼ��Ϊ(0,0,0)
    SEColorRGBA(float fR, float fG, float fB, float fA);
    SEColorRGBA(const float* pData);
    SEColorRGBA(const SEColorRGBA& rColor);

    inline operator const float* (void) const;
    inline operator float* (void);
    inline float operator [] (int i) const;
    inline float& operator [] (int i);

    SEColorRGBA& operator = (const SEColorRGBA& rColor);

    bool operator == (const SEColorRGBA& rColor) const;
    bool operator != (const SEColorRGBA& rColor) const;
    bool operator <  (const SEColorRGBA& rColor) const;
    bool operator <= (const SEColorRGBA& rColor) const;
    bool operator >  (const SEColorRGBA& rColor) const;
    bool operator >= (const SEColorRGBA& rColor) const;

    SEColorRGBA operator + (const SEColorRGBA& rColor) const;
    SEColorRGBA operator - (const SEColorRGBA& rColor) const;
    SEColorRGBA operator * (const SEColorRGBA& rColor) const;
    SEColorRGBA operator * (float fScalar) const;
    SE_CORE_API friend SEColorRGBA operator * (float fScalar, const 
        SEColorRGBA& rColor);

    SEColorRGBA& operator += (const SEColorRGBA& rColor);
    SEColorRGBA& operator -= (const SEColorRGBA& rColor);
    SEColorRGBA& operator *= (const SEColorRGBA& rColor);
    SEColorRGBA& operator *= (float fScalar);

    // ����ɫֵ�任��[0,1]����,
    // Clamp�����Ѹ����ü���0,����1�����ü���1,
    // ScaleByMax�����ٶ�������ɫͨ���Ǹ�,�ҵ�RGB������,����RGBֵ���Ը����ֵ,
    // Aֵ�ü���[0,1]����
    void Clamp(void);
    void ScaleByMax(void);

    static const SEColorRGBA SE_RGBA_BLACK;   // = (0,0,0,1) 
    static const SEColorRGBA SE_RGBA_WHITE;   // = (1,1,1,1)
    static const SEColorRGBA SE_RGBA_RED;     // = (1,0,0,1) 
    static const SEColorRGBA SE_RGBA_GREEN;   // = (0,1,0,1)
    static const SEColorRGBA SE_RGBA_BLUE;    // = (0,0,1,1)
    static const SEColorRGBA SE_RGBA_INVALID; // = (-1,-1,-1,-1)

private:
    inline int CompareData(const SEColorRGBA& rColor) const;
};

#include "SEColorRGBA.inl"

}

#endif
