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


	public:
		int								ShaderProgram;
		std::vector<ID3D11Buffer*>		ConstantBuffers;

	protected:
		D3D_FEATURE_LEVEL			m_FeatureLevel;
	};
};


//--------------------------------------------------------------------------------
#endif