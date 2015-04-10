//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkSpriteFont11.h
//--------------------------------------------------------------------------------
#ifndef __SpriteVertex11_h__
#define __SpriteVertex11_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class SpriteVertex11
	{

	public:
		SpriteVertex11();
		~SpriteVertex11();

		struct SpriteDrawRect
		{
			float X;
			float Y;
			float Width;
			float Height;
		};

		struct InstanceData
		{
			DirectX::XMFLOAT4X4 Transform;
			DirectX::XMFLOAT4 Color;
			SpriteDrawRect DrawRect;
		};

		struct VertexData
		{
			DirectX::XMFLOAT2 Position;
			DirectX::XMFLOAT2 TexCoord;
		};

		static unsigned int GetElementCount();
		static D3D11_INPUT_ELEMENT_DESC Elements[8];
	};
};
//--------------------------------------------------------------------------------
#endif // SpriteVertexDX11_h
//--------------------------------------------------------------------------------