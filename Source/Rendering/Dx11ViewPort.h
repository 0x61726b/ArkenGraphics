//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Dx11ViewPort.h
//--------------------------------------------------------------------------------
#ifndef __Dx11ViewPort_h__
#define __Dx11ViewPort_h__
//--------------------------------------------------------------------------------
#include "ArkRenderer11.h"
#include "DxRasterizerStage.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class Dx11ViewPort
	{
	public:
		Dx11ViewPort();
		Dx11ViewPort( D3D11_VIEWPORT v );
		~Dx11ViewPort();

		float GetWidth() const;
		float GetHeight() const;

	protected:
		D3D11_VIEWPORT		m_ViewPort;

		friend DxRasterizerStage;
		friend ArkRenderer11;
		

	};
};

#endif