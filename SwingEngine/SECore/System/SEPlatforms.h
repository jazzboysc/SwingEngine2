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

#ifndef Swing_Platforms_H
#define Swing_Platforms_H

#ifndef SE_L1_CACHE_LINE_SIZE
#define SE_L1_CACHE_LINE_SIZE 64
#endif

//----------------------------------------------------------------------------
// Common
//----------------------------------------------------------------------------
#ifndef SE_POINTER_SIZE
#if defined(__amd64__) || defined(_M_X64)
#define SE_POINTER_SIZE 8
#elif defined(__i386__) || defined(_M_IX86)
#define SE_POINTER_SIZE 4
#endif
#endif

#ifndef SE_HAS_64_BIT_ATOMICS
#if (SE_POINTER_SIZE == 8)
#define SE_HAS_64_BIT_ATOMICS
#endif
#endif
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Microsoft Windows 2000/XP/Vista/7 platform
//----------------------------------------------------------------------------
#if defined(_WIN32)

#define SE_IS_WINDOWS

#define SE_ALIGN1 __declspec(align(1))
#define SE_ALIGN2 __declspec(align(2))
#define SE_ALIGN4 __declspec(align(4))
#define SE_ALIGN8 __declspec(align(8))
#define SE_ALIGN16 __declspec(align(16))

#if defined(_MSC_VER)
// _MSC_VER 是 VC编译器内定义的一个宏
// 1200 VC6.0
// 1201 EVC4.2
// 1300 VC2002
// 1310 VC2003
// 1400 VC2005
#if _MSC_VER < 1300
#define SE_USING_VC6
#elif _MSC_VER < 1310
#define SE_USING_VC70
#elif _MSC_VER < 1400
#define SE_USING_VC71
#elif _MSC_VER < 1500
#define SE_USING_VC80
#elif _MSC_VER < 1600
#define SE_USING_VC90
#else
#define SE_USING_VC100
#endif 

// 使用SE_ASSERT( false )时,该宏展开后,可能产生条件表达式为常量的情况,
// 避免这个编译器4级警告.
#pragma warning(disable:4127)

// 声明类成员无名称struct时,避免这个编译器4级警告.
#pragma warning(disable:4201)

// The use of SE_CORE_API to export an entire class generates warnings when 
// member data and functions involving templates or inlines occur.  To avoid 
// the warning, SE_CORE_API can be applied only to those items that really 
// need to be exported.
#pragma warning(disable:4251)

// 含有const成员的类无法由编译器生成赋值运算符的警告.
#pragma warning(disable:4512)

#include <float.h>
#if _MSC_VER < 1800
#define isnan _isnan
#define isinf(f) (!_finite((f)))
#endif

typedef __int8           SE_Int8;
typedef unsigned __int8  SE_UInt8;
typedef __int16          SE_Int16;
typedef unsigned __int16 SE_UInt16;
typedef __int32          SE_Int32;
typedef unsigned __int32 SE_UInt32;
typedef __int64          SE_Int64;
typedef unsigned __int64 SE_UInt64;

#endif
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Macintosh OS X platform
//----------------------------------------------------------------------------
#elif defined(__APPLE__)

#define SE_IS_APPLE

#define SE_ALIGN1
#define SE_ALIGN2
#define SE_ALIGN4
#define SE_ALIGN8
#define SE_ALIGN16

#ifndef SE_L1_CACHE_LINE_SIZE
#define SE_L1_CACHE_LINE_SIZE 64
#endif

#if defined(__BIG_ENDIAN__)
#define SE_BIG_ENDIAN
#else
#define SE_LITTLE_ENDIAN
#endif

#include <stdint.h>
typedef int8_t   SE_Int8;
typedef uint8_t  SE_UInt8;
typedef int16_t  SE_Int16;
typedef uint16_t SE_UInt16;
typedef int32_t  SE_Int32;
typedef uint32_t SE_UInt32;
typedef int64_t  SE_Int64;
typedef uint64_t SE_UInt64;
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Linux platform
//----------------------------------------------------------------------------
#else

#define SE_IS_LINUX

#define SE_ALIGN1
#define SE_ALIGN2
#define SE_ALIGN4
#define SE_ALIGN8
#define SE_ALIGN16

#ifndef SE_L1_CACHE_LINE_SIZE
#define SE_L1_CACHE_LINE_SIZE 64
#endif

#include <stdint.h>
typedef int8_t   SE_Int8;
typedef uint8_t  SE_UInt8;
typedef int16_t  SE_Int16;
typedef uint16_t SE_UInt16;
typedef int32_t  SE_Int32;
typedef uint32_t SE_UInt32;
typedef int64_t  SE_Int64;
typedef uint64_t SE_UInt64;

#endif
//----------------------------------------------------------------------------

#endif
