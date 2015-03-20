//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkShaderReflectionFactory11.h
//--------------------------------------------------------------------------------
#ifndef __ArkShaderReflectionFactory11_h__
#define __ArkShaderReflectionFactory11_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkShader11.h"
#include "ArkShaderFactory11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkShaderReflectionFactory11
	{
	public:
		~ArkShaderReflectionFactory11();

		static ArkShaderReflection11* GenerateReflection(ArkShader11& shader);

	private:
		ArkShaderReflectionFactory11();
	};
};
//--------------------------------------------------------------------------------
#endif