//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkSceneGraph.h
//--------------------------------------------------------------------------------
#ifndef __ArkRay3_h__
#define __ArkRay3_h__
//--------------------------------------------------------------------------------
#include "ArkShape3D.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkRay3 : public ArkShape3D
	{
	public:
		ArkRay3( );
		ArkRay3( const DirectX::XMVECTOR& position, const DirectX::XMVECTOR& direction );
		virtual ~ArkRay3( );

		virtual eEShape GetShapeType( ) const;

	public:
		DirectX::XMVECTOR Origin;
		DirectX::XMVECTOR Direction;
	};
};
//--------------------------------------------------------------------------------
#endif // Ray3f_h
