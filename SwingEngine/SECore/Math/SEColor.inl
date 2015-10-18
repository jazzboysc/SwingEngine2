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

//----------------------------------------------------------------------------
inline unsigned int SEColor::MakeR8G8B8(unsigned char ucRed, 
    unsigned char ucGreen, unsigned char ucBlue)
{
#ifdef SE_BIG_ENDIAN
    return (255 | (ucBlue << 8) | (ucGreen << 16) | (ucRed << 24));
#else
    return (ucRed | (ucGreen << 8) | (ucBlue << 16) | (255 << 24));
#endif
}
//----------------------------------------------------------------------------
inline unsigned int SEColor::MakeR8G8B8A8(unsigned char ucRed,
    unsigned char ucGreen, unsigned char ucBlue, unsigned char ucAlpha)
{
#ifdef SE_BIG_ENDIAN
    return (ucAlpha | (ucBlue << 8) | (ucGreen << 16) | (ucRed << 24));
#else
    return (ucRed | (ucGreen << 8) | (ucBlue << 16) | (ucAlpha << 24));
#endif
}
//----------------------------------------------------------------------------
inline void SEColor::ExtractR8G8B8(unsigned int uiColor, unsigned char& rucRed,
    unsigned char& rucGreen, unsigned char& rucBlue)
{
#ifdef SE_BIG_ENDIAN
    rucRed =   (unsigned char)((uiColor & 0xFF000000) >> 24);
    rucGreen = (unsigned char)((uiColor & 0x00FF0000) >> 16);
    rucBlue =  (unsigned char)((uiColor & 0x0000FF00) >>  8);
#else
    rucRed =   (unsigned char)((uiColor & 0x000000FF));
    rucGreen = (unsigned char)((uiColor & 0x0000FF00) >>  8);
    rucBlue =  (unsigned char)((uiColor & 0x00FF0000) >> 16);
#endif
}
//----------------------------------------------------------------------------
inline void SEColor::ExtractR8G8B8A8(unsigned int uiColor, 
    unsigned char& rucRed, unsigned char& rucGreen, unsigned char& rucBlue, 
    unsigned char& rucAlpha)
{
#ifdef SE_BIG_ENDIAN
    rucRed =   (unsigned char)((uiColor & 0xFF000000) >> 24);
    rucGreen = (unsigned char)((uiColor & 0x00FF0000) >> 16);
    rucBlue =  (unsigned char)((uiColor & 0x0000FF00) >>  8);
    rucAlpha = (unsigned char)((uiColor & 0x000000FF));
#else
    rucRed =   (unsigned char)((uiColor & 0x000000FF));
    rucGreen = (unsigned char)((uiColor & 0x0000FF00) >>  8);
    rucBlue =  (unsigned char)((uiColor & 0x00FF0000) >> 16);
    rucAlpha = (unsigned char)((uiColor & 0xFF000000) >> 24);
#endif
}
//----------------------------------------------------------------------------
