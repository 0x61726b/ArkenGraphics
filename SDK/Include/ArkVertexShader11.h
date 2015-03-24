//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkVertexShader11.h
//--------------------------------------------------------------------------------
#ifndef __ArkVertexShader11_h__
#define __ArkVertexShader11_h__
//--------------------------------------------------------------------------------
#include "ArkShader11.h"
#include "PipelineManager.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkVertexShader11 : public ArkShader11
	{
	public:
		ArkVertexShader11( ID3D11VertexShader* pShader );
		virtual ~ArkVertexShader11();

		virtual ShaderType GetType();

	protected:
		ID3D11VertexShader*		m_pVertexShader;

		friend ArkVertexShaderStage11;

	};
};
#endif
//--------------------------------------------------------------------------------
