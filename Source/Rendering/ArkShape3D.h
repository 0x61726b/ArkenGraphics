//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkSceneGraph.h
//--------------------------------------------------------------------------------
#ifndef __ArkShape3D_h__
#define __ArkShape3D_h__
//--------------------------------------------------------------------------------
#include <DirectXMath.h>
//--------------------------------------------------------------------------------
namespace Arkeng
{
	enum eEShape
	{
		SPHERE,
		BOX,
		TRIANGLE,
		RAY,
		PLANE,
		FRUSTUM,
		CONE
	};

	class ArkShape3D
	{
	public:
		ArkShape3D( );
		virtual ~ArkShape3D( );

		virtual eEShape GetShapeType( ) const = 0;
	};
};
//--------------------------------------------------------------------------------
#endif // Shape3D_h
