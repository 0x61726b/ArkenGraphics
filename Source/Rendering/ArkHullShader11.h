//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkHullShader11.h
//--------------------------------------------------------------------------------
#ifndef __ArkHullShader11_h__
#define __ArkComputeShader11_h__
//--------------------------------------------------------------------------------
#include "ArkShader11.h"
#include "PipelineManager.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkHullShader11 : public ArkShader11
	{
	public:
		ArkHullShader11( ID3D11HullShader* pShader );
		virtual ~ArkHullShader11();

		virtual ShaderType GetType();

	protected:
		ID3D11HullShader*		m_pHullShader;

		friend ArkHullShaderStage11;

	};
};
#endif
//--------------------------------------------------------------------------------
