//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//BasicVertex11.h
//--------------------------------------------------------------------------------
#ifndef __BasicVertex11_h__
#define __BasicVertex11_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class BasicVertex11
	{
	public:
		BasicVertex11();
		~BasicVertex11();

		struct Vertex
		{
			DirectX::XMFLOAT3 Position;
			DirectX::XMFLOAT3 Normal;
			DirectX::XMFLOAT4 Color;
			DirectX::XMFLOAT2 TexCoords; 
		};

		static unsigned int GetElementCount();
		static D3D11_INPUT_ELEMENT_DESC Elements[4];
	};
};
//--------------------------------------------------------------------------------
#endif
