//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkVertexShader.h
//--------------------------------------------------------------------------------
#ifndef  __ArkShaderStage11_h__
#define  __ArkShaderStage11_h__
//--------------------------------------------------------------------------------
#include "ArkShader11.h"
#include "ArkShaderStageState11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkShaderStage11
	{
	public:
		ArkShaderStage11();
		virtual ~ArkShaderStage11();

		void SetFeatureLevel(D3D_FEATURE_LEVEL level);

		void ClearState();
		void ApplyState(ID3D11DeviceContext* pContext);

		virtual ShaderType GetType() = 0;

		virtual void BindShaderProgram(ID3D11DeviceContext*) = 0;
		virtual void BindConstantBuffers(ID3D11DeviceContext* pContext) = 0;


		ArkShaderStageState11 State;
	protected:

		D3D_FEATURE_LEVEL			m_FeatureLevel;
	};
};
//--------------------------------------------------------------------------------
#endif