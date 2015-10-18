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

#ifndef Swing_NameIdMCR_H
#define Swing_NameIdMCR_H

// 一般情况下,使用者必须实现各自的GetObjectByName,GetAllObjectsByName,
// GetObjectByID函数,且这些函数要首先调用基类的相应函数.
//----------------------------------------------------------------------------
#define SE_DECLARE_NAME_ID \
public: \
    virtual SEObject* GetObjectByName(const std::string& rName); \
    virtual void GetAllObjectsByName(const std::string& rName, \
        std::vector<SEObject*>& rObjects); \
    virtual SEObject* GetObjectByID(unsigned int uiID)
//----------------------------------------------------------------------------
#define SE_IMPLEMENT_DEFAULT_NAME_ID(classname, baseclassname) \
SEObject* classname::GetObjectByName(const std::string& rName) \
{ \
    return baseclassname::GetObjectByName(rName); \
} \
\
void classname::GetAllObjectsByName(const std::string& rName, \
    std::vector<SEObject*>& rObjects) \
{ \
    baseclassname::GetAllObjectsByName(rName, rObjects); \
} \
\
SEObject* classname::GetObjectByID(unsigned int uiID) \
{ \
    return baseclassname::GetObjectByID(uiID); \
}
//----------------------------------------------------------------------------

#endif
