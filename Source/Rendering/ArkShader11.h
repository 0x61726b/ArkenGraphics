//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkShader11.h
//--------------------------------------------------------------------------------
#ifndef __ArkShader11_h__
#define __ArkShader11_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkShaderReflection11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkShader11
	{
		public:
		ArkShader11();
		virtual ~ArkShader11();

		virtual ShaderType GetType() = 0;
		

		void SetReflection( ArkShaderReflection11* pReflection );
		ArkShaderReflection11* GetReflection( );

		std::wstring ToString();

	public:
		std::wstring							FileName;
		std::wstring							Function;
		std::wstring							ShaderModel;
		std::string								ShaderText;
		ID3DBlob*								m_pCompiledShader;
		ArkShaderReflection11*					m_pReflection;
	};
};
//--------------------------------------------------------------------------------
#endif
