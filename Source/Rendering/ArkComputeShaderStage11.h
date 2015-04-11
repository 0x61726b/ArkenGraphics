//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkComputeShaderStage11.h
//--------------------------------------------------------------------------------
#ifndef __ArkComputeShaderStage11_h__
#define __ArkComputeShaderStage11_h__
//--------------------------------------------------------------------------------
#include "ArkShaderStage11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkComputeShaderStage11 : public ArkShaderStage11
	{
	public:
		ArkComputeShaderStage11();
		virtual ~ArkComputeShaderStage11();

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