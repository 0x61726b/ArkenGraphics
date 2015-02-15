//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Dx11ViewPort.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "Dx11ViewPort.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
Dx11ViewPort::Dx11ViewPort()
{
}
//--------------------------------------------------------------------------------
Dx11ViewPort::Dx11ViewPort( D3D11_VIEWPORT ViewPort )
{
	m_ViewPort = ViewPort;
}
//--------------------------------------------------------------------------------
Dx11ViewPort::~Dx11ViewPort()
{
}
//--------------------------------------------------------------------------------
float Dx11ViewPort::GetHeight() const
{
	return m_ViewPort.Height;
}
//--------------------------------------------------------------------------------
float Dx11ViewPort::GetWidth() const
{
	return m_ViewPort.Width;
}
//--------------------------------------------------------------------------------
