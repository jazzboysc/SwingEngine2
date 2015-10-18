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

#ifndef Swing_Light_H
#define Swing_Light_H

#include "SERenderingEngineLIB.h"
#include "SEAdvancedObject.h"
#include "SEColorRGB.h"
#include "SEVector3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20080702
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SELight : public SEAdvancedObject
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    enum LightType
    {
        LT_AMBIENT,
        LT_DIRECTIONAL,
        LT_POINT,
        LT_SPOT,
        LT_COUNT
    };

    SELight(LightType eType = LT_AMBIENT);
    virtual ~SELight(void);

    // 如果SELight类没有数据,或只有环境光颜色和强度,
    // 则可以使用一个标准的类层级体系:
    //
    //   class SELight
    //       [ambient, intensity]
    //   class SEAmbientLight : public SELight
    //       [没有额外数据]
    //   class SEDirectionalLight : public SELight
    //       [direction, diffuse, specular]
    //   class SEPointLight : public SELight
    //       [position, diffuse, specular, attenuation]
    //   class SESpotLight : public SEPointLight
    //       [cone axis, cone angle, spot exponent]
    //
    // 渲染器通过基类SELight来把握所有light类型.
    // 使用以上标准类层级体系,渲染器必须通过动态类型转换来判断具体的灯光类型,
    // 之后根据具体灯光类型,通过SERenderer::SetConstantLightFOOBAR系列函数,
    // 设置shader程序常量,希望能够避免这种低效率的做法.
    //
    // 另一种做法是,允许SELight类在public作用域存储所有灯光所需的数据,
    // 使用一个被保护的基类SELight来派生出特定的SELight子类.
    // 因此,渲染器在不使用动态类型转换的情况下可以访问到所有灯光数据,
    // 并且派生类通过类成员函数来访问与其类型相关的灯光数据.
    // 但不幸的是,你会遇到SEObject类成员访问权受限的问题(例如智能指针增减索
    // 引计数).
    //
    // 最终,还是使用了完全开放的策略,
    // SELight类作为一个通用类,存储了派生类所需的各种数据.

    LightType Type;     // default: LT_AMBIENT

    // The colors of the light.
    SEColorRGB Ambient;   // default: SEColorRGB(0,0,0)
    SEColorRGB Diffuse;   // default: SEColorRGB(0,0,0)
    SEColorRGB Specular;  // default: SEColorRGB(0,0,0)

    // Attenuation is typically specified as a modulator
    //   m = 1/(C + L*d + Q*d*d)
    // where C is the constant coefficient, L is the linear coefficient,
    // Q is the quadratic coefficient, and d is the distance from the light
    // position to the vertex position.  To allow for a linear adjustment of
    // intensity, my choice is to use instead
    //   m = I/(C + L*d + Q*d*d)
    // where I is an "intensity" factor.
    float Constant;     // default: 1
    float Linear;       // default: 0
    float Quadratic;    // default: 0
    float Intensity;    // default: 1

    // Parameters for spot lights.  The cone angle must be in radians and
    // should satisfy 0 < Angle <= pi.
    float Angle;        // default: pi
    float CosAngle;     // default: -1
    float SinAngle;     // default:  0
    float Exponent;     // default:  1

    // A helper function that lets you set Angle and have CosAngle and
    // SinAngle computed for you.
    void SetAngle(float fAngle);

    // 尽管标准的方向光和聚光灯都只需要一个方向向量,
    // 但为了支持使用完整坐标系的新的派生类型,还是在SELight基类中存储了这个
    // 坐标系.
    // 灯光坐标系表示总是基于世界坐标系.
    //   default position  P = (0,0,0)
    //   default right     R = (1,0,0)
    //   default up        U = (0,1,0)
    //   default direction D = (0,0,1)
    // {P:R,U,D}构成左手坐标系.
    SEVector3f Position, RVector, UVector, DVector;

    // 设置方向向量D,同时自动计算并调整R,U.
    void SetDirection(const SEVector3f& rDirection, bool bUnitLength = true);

    // 在debug模式使用,判断是否为一个有效的左手规范坐标基.
    bool IsValidFrame(void) const;

protected:
};

typedef SESmartPointer<SELight> SELightPtr;

}

#endif
