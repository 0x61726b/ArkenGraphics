//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkBox.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkBox.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
using namespace DirectX;
//--------------------------------------------------------------------------------
ArkBox::ArkBox()
{
	m_vCenter = XMVectorZero();
	m_vExtents = XMVectorZero();
	m_vMax = XMVectorZero();
	m_vMin = XMVectorZero();
}
//--------------------------------------------------------------------------------
ArkBox::ArkBox( XMVECTOR& Center, XMVECTOR& Extents,XMVECTOR& Max,XMVECTOR& Min )
{
	m_vCenter = Center;
	m_vExtents = Extents;
	m_vMax = Max;
	m_vMin = Min;
}
//--------------------------------------------------------------------------------
ArkBox::ArkBox( XMVECTOR& Min, XMVECTOR& Max)
{
	m_vMin = Min;
	m_vMax = Max;

	m_vCenter = ( m_vMin + m_vMax )* 0.5f;
	m_vExtents = ( m_vMax - m_vMin )* 0.5f;

}
//--------------------------------------------------------------------------------
ArkBox::~ArkBox()
{

}
//--------------------------------------------------------------------------------
DirectX::XMVECTOR ArkBox::Center() const
{
	return m_vCenter;
}
//--------------------------------------------------------------------------------
DirectX::XMVECTOR& ArkBox::Center()
{
	return m_vCenter;
}
//--------------------------------------------------------------------------------
DirectX::XMVECTOR ArkBox::Extents() const
{
	return m_vExtents;
}
//--------------------------------------------------------------------------------
DirectX::XMVECTOR& ArkBox::Extents()
{
	return m_vExtents;
}
//--------------------------------------------------------------------------------
DirectX::XMVECTOR ArkBox::Max() const
{
	return m_vMax;
}
//--------------------------------------------------------------------------------
DirectX::XMVECTOR& ArkBox::Max()
{
	return m_vMax;
}
//--------------------------------------------------------------------------------
DirectX::XMVECTOR ArkBox::Min() const
{
	return m_vMin;
}
//--------------------------------------------------------------------------------
DirectX::XMVECTOR& ArkBox::Min()
{
	return m_vMin;
}
//--------------------------------------------------------------------------------
eEShape ArkBox::GetShapeType() const
{
	return eEShape::BOX;
}
