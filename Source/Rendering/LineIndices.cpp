//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//LineIndices.cpp
//--------------------------------------------------------------------------------
#include "PCH.h"
#include "LineIndices.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
LineIndices::LineIndices()
{
	m_uiIndices[0] = 0;
	m_uiIndices[1] = 0;
}
//--------------------------------------------------------------------------------
LineIndices::LineIndices( unsigned int P1, unsigned int P2 )
{
	m_uiIndices[0] = P1;
	m_uiIndices[1] = P2;
}
//--------------------------------------------------------------------------------
LineIndices::~LineIndices()
{
}
//--------------------------------------------------------------------------------
LineIndices& LineIndices::operator= ( const LineIndices& Line )
{
	m_uiIndices[0] = Line.m_uiIndices[0];
	m_uiIndices[1] = Line.m_uiIndices[1];

	return( *this );
}
//--------------------------------------------------------------------------------
unsigned int LineIndices::P1() const
{
	return( m_uiIndices[0] );
}
//--------------------------------------------------------------------------------
unsigned int& LineIndices::P1()
{
	return( m_uiIndices[0] );
}
//--------------------------------------------------------------------------------
unsigned int LineIndices::P2() const
{
	return( m_uiIndices[1] );
}
//--------------------------------------------------------------------------------
unsigned int& LineIndices::P2()
{
	return( m_uiIndices[1] );
}
//--------------------------------------------------------------------------------
void LineIndices::swapP1P2( )
{
	unsigned int swap = m_uiIndices[0];
	m_uiIndices[0] = m_uiIndices[1];
	m_uiIndices[1] = swap;
}
//--------------------------------------------------------------------------------
