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
inline const SERTTI& SEObject::GetType() const
{
    return TYPE;
}
//----------------------------------------------------------------------------
inline bool SEObject::IsExactly(const SERTTI& rType) const
{
    return GetType().IsExactly(rType);
}
//----------------------------------------------------------------------------
inline bool SEObject::IsDerived(const SERTTI& rType) const
{
    return GetType().IsDerived(rType);
}
//----------------------------------------------------------------------------
inline bool SEObject::IsExactlyTypeOf(const SEObject* pObject) const
{
    return pObject && GetType().IsExactly(pObject->GetType());
}
//----------------------------------------------------------------------------
inline bool SEObject::IsDerivedTypeOf(const SEObject* pObject) const
{
    return pObject && GetType().IsDerived(pObject->GetType());
}
//----------------------------------------------------------------------------
template <class T>
T* StaticCast(SEObject* pObject)
{
    return (T*)pObject;
}
//----------------------------------------------------------------------------
template <class T>
const T* StaticCast(const SEObject* pObject)
{
    return (const T*)pObject;
}
//----------------------------------------------------------------------------
template <class T>
T* DynamicCast(SEObject* pObject)
{
    return pObject && pObject->IsDerived(T::TYPE) ? (T*)pObject : 0;
}
//----------------------------------------------------------------------------
template <class T>
const T* DynamicCast(const SEObject* pObject)
{
    return pObject && pObject->IsDerived(T::TYPE) ? (const T*)pObject : 0;
}
//----------------------------------------------------------------------------
inline void SEObject::SetName(const std::string& rName)
{
    mName = rName;
}
//----------------------------------------------------------------------------
inline const std::string& SEObject::GetName() const
{
    return mName;
}
//----------------------------------------------------------------------------
inline unsigned int SEObject::GetID() const
{
    return mID;
}
//----------------------------------------------------------------------------
inline unsigned int SEObject::GetNextID()
{
    return msNextID;
}
//----------------------------------------------------------------------------
