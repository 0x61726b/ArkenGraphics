//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkDomainShaderStage11.h
//--------------------------------------------------------------------------------
#ifndef __ArkDomainShaderStage11_h__
#define __ArkDomainShaderStage11_h__
//--------------------------------------------------------------------------------
#include "ArkShaderStage11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkDomainShaderStage11 : public ArkShaderStage11
	{
	public:
		ArkDomainShaderStage11();
		virtual ~ArkDomainShaderStage11();

	protected:
		virtual ShaderType GetType();

		
		virtual void BindShaderProgram( ID3D11DeviceContext* );
		virtual void BindConstantBuffers( ID3D11DeviceContext* pContext, int count );
		virtual void BindSamplerStates( ID3D11DeviceContext* pContext, int count );
		virtual void BindShaderResourceViews( ID3D11DeviceContext* pContext, int count );
		virtual void BindUnorderedAccessViews( ID3D11DeviceContext* pContext, int count );
	};
};
//--------------------------------------------------------------------------------
#endif