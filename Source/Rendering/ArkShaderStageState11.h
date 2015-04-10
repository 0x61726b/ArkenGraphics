//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkShaderStageState11.h
//--------------------------------------------------------------------------------
#ifndef __ArkShaderStageState11_h__
#define __ArkShaderStageState11_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "Dx11ResourceProxy.h"
#include "TStateMonitor.h"
#include "TStateArrayMonitor.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkShaderStageState11
	{
	public:
		ArkShaderStageState11();
		virtual ~ArkShaderStageState11();

		void SetFeautureLevel(D3D_FEATURE_LEVEL level);


		void ClearState();
		void SetPreviousState( ArkShaderStageState11* pPrev );
		void ResetUpdate();

		TStateMonitor<int>													ShaderProgram;
		TStateArrayMonitor<ID3D11Buffer*,
			D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT>				ConstantBuffers;
		TStateArrayMonitor< ID3D11SamplerState*, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT > SamplerStates;
		TStateArrayMonitor< ID3D11ShaderResourceView*, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT > ShaderResourceViews;

	protected:
		D3D_FEATURE_LEVEL			m_FeatureLevel;

		ArkShaderStageState11*		m_pPrevState;
	};
};


//--------------------------------------------------------------------------------
#endif