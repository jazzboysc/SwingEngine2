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

#ifndef Swing_ColorRGB_H
#define Swing_ColorRGB_H

#include "SECoreLIB.h"
#include "SEPlatforms.h"
#include "SESystem.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20080312
//----------------------------------------------------------------------------
class SE_CORE_API SEColorRGB
{
public:
    union
    {
        struct
		{
            float R;
            float G;
            float B;
		};
        float m_fData[3];
    };

public:
    // RGBֵ��Χ��[0,1]֮��
    SEColorRGB(void);  // ��ʼ��Ϊ(0,0,0)
    SEColorRGB(float fR, float fG, float fB);
    SEColorRGB(const float* pData);
    SEColorRGB(const SEColorRGB& rColor);

    // ��Ա����
    inline operator const float* (void) const;
    inline operator float* (void);
    inline float operator [] (int i) const;
    inline float& operator [] (int i);

    SEColorRGB& operator = (const SEColorRGB& rColor);

    bool operator == (const SEColorRGB& rColor) const;
    bool operator != (const SEColorRGB& rColor) const;
    bool operator <  (const SEColorRGB& rColor) const;
    bool operator <= (const SEColorRGB& rColor) const;
    bool operator >  (const SEColorRGB& rColor) const;
    bool operator >= (const SEColorRGB& rColor) const;

    SEColorRGB operator + (const SEColorRGB& rColor) const;
    SEColorRGB operator - (const SEColorRGB& rColor) const;
    SEColorRGB operator * (const SEColorRGB& rColor) const;
    SEColorRGB operator * (float fScalar) const;
    SE_CORE_API friend SEColorRGB operator * (float fScalar, const 
        SEColorRGB& rColor);

    SEColorRGB& operator += (const SEColorRGB& rColor);
    SEColorRGB& operator -= (const SEColorRGB& rColor);
    SEColorRGB& operator *= (const SEColorRGB& rColor);
    SEColorRGB& operator *= (float fScalar);

    // ����ɫֵ�任��[0,1]����,
    // Clamp�����Ѹ����ü���0,����1�����ü���1,
    // ScaleByMax�����ٶ�������ɫͨ���Ǹ�,�ҵ�������,��������ֵ���Ը����ֵ
    void Clamp(void);
    void ScaleByMax(void);

    static const SEColorRGB SE_RGB_BLACK;    // = (0,0,0) 
    static const SEColorRGB SE_RGB_WHITE;    // = (1,1,1)
    static const SEColorRGB SE_RGB_RED;      // = (1,0,0)
    static const SEColorRGB SE_RGB_GREEN;    // = (0,1,0)
    static const SEColorRGB SE_RGB_BLUE;     // = (0,0,1)
    static const SEColorRGB SE_RGB_INVALID;  // = (-1,-1,-1)

private:
    inline int CompareData(const SEColorRGB& rColor) const;
};

#include "SEColorRGB.inl"

}

#endif
