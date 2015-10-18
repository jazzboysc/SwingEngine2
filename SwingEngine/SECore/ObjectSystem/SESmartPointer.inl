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
template <class T>
SESmartPointer<T>::SESmartPointer(T* pObject)
{
    mObject = pObject;
    if( mObject )
    {
        mObject->IncrementReferenceCount();
    }
}
//----------------------------------------------------------------------------
template <class T>
SESmartPointer<T>::SESmartPointer(const SESmartPointer& rSmartPointer)
{
    mObject = rSmartPointer.mObject;
    if( mObject )
    {
        mObject->IncrementReferenceCount();
    }
}
//----------------------------------------------------------------------------
template <class T>
SESmartPointer<T>::~SESmartPointer()
{
    if( mObject )
    {
        mObject->DecrementReferenceCount();
        mObject = 0;
    }
}
//----------------------------------------------------------------------------
template <class T>
SESmartPointer<T>::operator T*() const
{
    return mObject;
}
//----------------------------------------------------------------------------
template <class T>
T& SESmartPointer<T>::operator*() const
{
    return *mObject;
}
//----------------------------------------------------------------------------
template <class T>
T* SESmartPointer<T>::operator->() const
{
    return mObject;
}
//----------------------------------------------------------------------------
template <class T>
SESmartPointer<T>& SESmartPointer<T>::operator=(T* pObject)
{
    if( mObject != pObject )
    {
        if( pObject )
        {
            pObject->IncrementReferenceCount();
        }

        if( mObject )
        {
            mObject->DecrementReferenceCount();
        }

        mObject = pObject;
    }

    return *this;
}
//----------------------------------------------------------------------------
template <class T>
SESmartPointer<T>& SESmartPointer<T>::operator=(
    const SESmartPointer& rSmartPointer)
{
    if( mObject != rSmartPointer.mObject )
    {
        if( rSmartPointer.mObject )
        {
            rSmartPointer.mObject->IncrementReferenceCount();
        }

        if( mObject )
        {
            mObject->DecrementReferenceCount();
        }

        mObject = rSmartPointer.mObject;
    }

    return *this;
}
//----------------------------------------------------------------------------
template <class T>
bool SESmartPointer<T>::operator==(T* pObject) const
{
    return (mObject == pObject);
}
//----------------------------------------------------------------------------
template <class T>
bool SESmartPointer<T>::operator!=(T* pObject) const
{
    return (mObject != pObject);
}
//----------------------------------------------------------------------------
template <class T>
bool SESmartPointer<T>::operator==(const SESmartPointer& rSmartPointer) const
{
    return (mObject == rSmartPointer.mObject);
}
//----------------------------------------------------------------------------
template <class T>
bool SESmartPointer<T>::operator!=(const SESmartPointer& rSmartPointer) const
{
    return (mObject != rSmartPointer.mObject);
}
//----------------------------------------------------------------------------



