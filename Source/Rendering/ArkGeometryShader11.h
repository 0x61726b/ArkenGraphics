//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkGeometryShader11.h
//--------------------------------------------------------------------------------
#ifndef __ArkGeometryShader11_h__
#define __ArkGeometryShader11_h__
//--------------------------------------------------------------------------------
#include "ArkShader11.h"
#include "PipelineManager.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkGeometryShader11 : public ArkShader11
	{
	public:
		ArkGeometryShader11( ID3D11GeometryShader* pShader );
		virtual ~ArkGeometryShader11();

		virtual ShaderType GetType();

	protected:
		ID3D11GeometryShader*		m_pGeometryShader;

		friend ArkGeometryShaderStage11;

	};
};
#endif
//--------------------------------------------------------------------------------
