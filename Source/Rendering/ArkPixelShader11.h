//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkVertexShader11.h
//--------------------------------------------------------------------------------
#ifndef __ArkPixelShader11_h__
#define __ArkPixelShader11_h__
//--------------------------------------------------------------------------------
#include "ArkShader11.h"
#include "PipelineManager.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkPixelShader11 : public ArkShader11
	{
	public:
		ArkPixelShader11( ID3D11PixelShader* pShader );
		virtual ~ArkPixelShader11();

		virtual ShaderType GetType();

	protected:
		ID3D11PixelShader*		m_pPixelShader;

		friend ArkPixelShaderStage11;

	};
};
#endif
//--------------------------------------------------------------------------------
