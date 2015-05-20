//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkSurfaceMaterial.h
//--------------------------------------------------------------------------------
#include "Pch.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	struct MaterialElements
	{
		DirectX::XMFLOAT4 Ambient;
		DirectX::XMFLOAT4 Diffuse;
		DirectX::XMFLOAT4 Specular;
		DirectX::XMFLOAT4 Emissive;
		float			  Transparency;
		float			  Shininess;
		
	};
	struct MaterialTextures
	{
		std::string Diffuse;
		std::string Specular;
		std::string Emissive;
		std::string SpecularFactor;
		std::string Bump;
		std::string BumpFactor;
		std::string Transparency;
	};

	class ArkSurfaceMaterial
	{
		ArkSurfaceMaterial();
		~ArkSurfaceMaterial();
	};
}