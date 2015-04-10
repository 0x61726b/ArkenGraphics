//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkVertexShaderStage11.h
//--------------------------------------------------------------------------------
#ifndef __ArkPixelShaderStage11_h__
#define __ArkPixelShaderStage11_h__
//--------------------------------------------------------------------------------
#include "ArkShaderStage11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkPixelShaderStage11 : public ArkShaderStage11
	{
	public:
		ArkPixelShaderStage11();
		virtual ~ArkPixelShaderStage11();

	protected:
		virtual ShaderType GetType();

		virtual void BindShaderProgram(ID3D11DeviceContext*);
		virtual void BindConstantBuffers(ID3D11DeviceContext* pContext,int);
		virtual void BindSamplerStates( ID3D11DeviceContext* pContext, int count );
		virtual void BindShaderResourceViews( ID3D11DeviceContext* pContext, int count );
	};
};

//--------------------------------------------------------------------------------
#endif