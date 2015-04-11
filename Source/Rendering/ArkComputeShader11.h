//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkComputeShader11.h
//--------------------------------------------------------------------------------
#ifndef __ArkComputeShader11_h__
#define __ArkComputeShader11_h__
//--------------------------------------------------------------------------------
#include "ArkShader11.h"
#include "PipelineManager.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkComputeShader11 : public ArkShader11
	{
	public:
		ArkComputeShader11( ID3D11ComputeShader* pShader );
		virtual ~ArkComputeShader11();

		virtual ShaderType GetType();

	protected:
		ID3D11ComputeShader*		m_pComputeShader;

		friend ArkComputeShaderStage11;

	};
};
#endif
//--------------------------------------------------------------------------------
