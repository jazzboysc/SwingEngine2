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
void Allocate(int iCols, int iRows, T**& raatArray)
{
    raatArray = SE_NEW T*[iRows];
    raatArray[0] = SE_NEW T[iRows*iCols];
    for( int iRow = 1; iRow < iRows; iRow++ )
    {
        raatArray[iRow] = &raatArray[0][iCols*iRow];
    }
}
//----------------------------------------------------------------------------
template <class T>
void Deallocate(T**& raatArray)
{
    if( raatArray )
    {
        SE_DELETE[] raatArray[0];
        SE_DELETE[] raatArray;
        raatArray = 0;
    }
}
//----------------------------------------------------------------------------
template <class T>
void Allocate(int iCols, int iRows, int iSlices, T***& raaatArray)
{
    raaatArray = SE_NEW T**[iSlices];
    raaatArray[0] = SE_NEW T*[iSlices*iRows];
    raaatArray[0][0] = SE_NEW T[iSlices*iRows*iCols];
    for( int iSlice = 0; iSlice < iSlices; iSlice++ )
    {
        raaatArray[iSlice] = &raaatArray[0][iRows*iSlice];
        for( int iRow = 0; iRow < iRows; iRow++ )
        {
            raaatArray[iSlice][iRow] = &raaatArray[0][0][iCols*(iRow+
                iRows*iSlice)];
        }
    }
}
//----------------------------------------------------------------------------
template <class T>
void Deallocate(T***& raaatArray)
{
    if( raaatArray )
    {
        SE_DELETE[] raaatArray[0][0];
        SE_DELETE[] raaatArray[0];
        SE_DELETE[] raaatArray;
        raaatArray = 0;
    }
}
//----------------------------------------------------------------------------
