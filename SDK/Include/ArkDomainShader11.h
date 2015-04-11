//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkDomainShader11.h
//--------------------------------------------------------------------------------
#ifndef __ArkDomainShader11_h__
#define __ArkDomainShader11_h__
//--------------------------------------------------------------------------------
#include "ArkShader11.h"
#include "PipelineManager.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkDomainShader11 : public ArkShader11
	{
	public:
		ArkDomainShader11( ID3D11DomainShader* pShader );
		virtual ~ArkDomainShader11();

		virtual ShaderType GetType();

	protected:
		ID3D11DomainShader*		m_pDomainShader;

		friend ArkDomainShaderStage11;

	};
};
#endif
//--------------------------------------------------------------------------------
