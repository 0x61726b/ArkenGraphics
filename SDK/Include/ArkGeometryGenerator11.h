//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkGeometryGenerator11.h
//--------------------------------------------------------------------------------
#ifndef __ArkGeometryGenerator11_h__
#define __ArkGeometryGenerator11_h__
//--------------------------------------------------------------------------------
#include "ArkGeometry11.h"
//#include "SkinnedActor.h"
#include <string>
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkGeometryGenerator11
	{
	public:
		static void GenerateFullScreenQuad( GeometryPtr pGeometry );
		static void GenerateTexturedPlane( GeometryPtr pGeometry, int SizeX, int SizeY );
		static void GenerateAxisGeometry( GeometryPtr pGeometry );
        static void GenerateSphere( GeometryPtr pGeometry, unsigned int URes, 
                                    unsigned int VRes, float Radius );
        static void GenerateCone( GeometryPtr pGeometry, unsigned int URes, 
                                    unsigned int VRes, float Radius, float Height );
		static void GenerateCube( GeometryPtr pGeometry, int w,int h,int d );

	private:
		ArkGeometryGenerator11();
		
	};
};
#endif // GeometryGeneratorDX11_h


