//--------------------------------------------------------------------------------
// This file is a portion of the Hieroglyph 3 Rendering Engine.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Jason Zink 
//--------------------------------------------------------------------------------
#ifndef __ArkSphere3_h__
#define __ArkSphere3_h__
//--------------------------------------------------------------------------------
#include "ArkShape3D.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkSphere3 : public ArkShape3D
	{
	public:
		ArkSphere3( );
		ArkSphere3( const DirectX::XMVECTOR& center, float radius );
		virtual ~ArkSphere3( );

		void Update( const DirectX::XMVECTOR& center, float radius );
		void UpdateCenter( const DirectX::XMVECTOR& center );
		void UpdateRadius( float radius );

		bool Intersects( const ArkSphere3& test ) const;
		bool Envelops( const ArkSphere3& test ) const;

		void SamplePosition( DirectX::XMVECTOR& position, float theta, float phi ) const;
		void SampleNormal( DirectX::XMVECTOR& normal, float theta, float phi ) const;
		void SamplePositionAndNormal( DirectX::XMVECTOR& position, DirectX::XMVECTOR& normal, float theta, float phi ) const;

		virtual eEShape GetShapeType( ) const;

		DirectX::XMVECTOR	Center;
		float				Radius;
	};
};
//--------------------------------------------------------------------------------
#endif // Sphere3f_h
