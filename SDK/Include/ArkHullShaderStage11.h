//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkHullShaderStage11.h
//--------------------------------------------------------------------------------
#ifndef __ArkHullShaderStage11_h__
#define __ArkHullShaderStage11_h__
//--------------------------------------------------------------------------------
#include "ArkShaderStage11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkHullShaderStage11 : public ArkShaderStage11
	{
	public:
		ArkHullShaderStage11();
		virtual ~ArkHullShaderStage11();

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