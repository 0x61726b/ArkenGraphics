//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//BasicVertex11.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "BasicVertex11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
D3D11_INPUT_ELEMENT_DESC BasicVertex11::Elements[4] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
//--------------------------------------------------------------------------------
BasicVertex11::BasicVertex11()
{
}
//--------------------------------------------------------------------------------
BasicVertex11::~BasicVertex11()
{
}
//--------------------------------------------------------------------------------
unsigned int BasicVertex11::GetElementCount()
{
	return( sizeof( Elements ) / sizeof( Elements[0] ) );
}
//--------------------------------------------------------------------------------
