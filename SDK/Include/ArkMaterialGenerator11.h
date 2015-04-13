//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkGeometryGenerator11.h
//--------------------------------------------------------------------------------
#ifndef __ArkMaterialGenerator11_h__
#define __ArkMaterialGenerator11_h__
//--------------------------------------------------------------------------------
#include "ArkMaterial11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkMaterialGenerator11
	{
	public:
		static MaterialPtr GenerateWireFrame( ArkRenderer11& Renderer );

		
		static MaterialPtr GenerateStaticTextured( ArkRenderer11& Renderer );
		static MaterialPtr GenerateSkinnedTextured( ArkRenderer11& Renderer );
		static MaterialPtr GenerateSkinnedSolid( ArkRenderer11& Renderer );

		static MaterialPtr GeneratePhong( ArkRenderer11& Renderer );
		static MaterialPtr GenerateSolidColor( ArkRenderer11& Renderer );

		static MaterialPtr GenerateFromFile( ArkRenderer11& Renderer, std::wstring& file, unsigned int shaders );

		static MaterialPtr GenerateKinectReconstructionMaterial( ArkRenderer11& Renderer );
		static MaterialPtr GenerateKinectDepthBufferMaterial( ArkRenderer11& Renderer );

		static MaterialPtr GenerateImmediateGeometrySolidMaterial( ArkRenderer11& Renderer );
		static MaterialPtr GenerateImmediateGeometryTexturedMaterial( ArkRenderer11& Renderer );

		static MaterialPtr GenerateTextMaterial( ArkRenderer11& Renderer );

		static MaterialPtr GenerateVolumeGeometryMaterial( ArkRenderer11& Renderer );

	private:
		ArkMaterialGenerator11();
		
	};
};
#endif // MaterialGeneratorDX11_h


