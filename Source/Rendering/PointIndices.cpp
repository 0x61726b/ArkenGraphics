//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//PointIndices.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "PointIndices.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
PointIndices::PointIndices()
{
	m_uiIndices[0] = 0;
}
//--------------------------------------------------------------------------------
PointIndices::PointIndices( unsigned int P1 )
{
	m_uiIndices[0] = P1;
}
//--------------------------------------------------------------------------------
PointIndices::~PointIndices()
{
}
//--------------------------------------------------------------------------------
PointIndices& PointIndices::operator= (const PointIndices& Point)
{
	m_uiIndices[0] = Point.m_uiIndices[0];

	return( *this );
}
//--------------------------------------------------------------------------------
unsigned int PointIndices::P1() const
{
	return( m_uiIndices[0] );
}
//--------------------------------------------------------------------------------
unsigned int& PointIndices::P1()
{
	return( m_uiIndices[0] );
}
//--------------------------------------------------------------------------------
