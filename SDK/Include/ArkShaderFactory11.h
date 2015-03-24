//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkShaderFactory11.h
//--------------------------------------------------------------------------------
#ifndef __ArkShaderFactory11_h__
#define __ArkShaderFactory11_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkShader11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkShaderFactory11
	{
	public:
		~ArkShaderFactory11();

		static ID3DBlob* GenerateShader( ShaderType type, std::wstring& filename, std::wstring& function,
            std::wstring& model, const D3D_SHADER_MACRO* pDefines, bool enablelogging = true );

		static ID3DBlob* GeneratePrecompiledShader( std::wstring& filename, std::wstring& function,
            std::wstring& model );

	private:
		ArkShaderFactory11();
	};
};


#endif
//--------------------------------------------------------------------------------
