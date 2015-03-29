//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//X.h
//--------------------------------------------------------------------------------
#ifndef __ArkFrustum_h__
#define __ArkFrustum_h__
//--------------------------------------------------------------------------------
#include "ArkShape3D.h"
#include "ArkPlane.h"
#include "ArkSphere3.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkFrustum : public ArkShape3D
	{
	public:
		ArkFrustum();
		ArkFrustum( const DirectX::XMMATRIX& ViewProjection );
		virtual ~ArkFrustum();

		void Update( const DirectX::XMMATRIX& ViewProjection, bool bNormalize );
		bool Test( const DirectX::XMVECTOR& TestPoint ) const;
		bool Test( const ArkSphere3& TestSphere ) const;

		// for use with the CEntity interface
		bool Intersects( const ArkSphere3& test ) const;
		bool Envelops( const ArkSphere3& test ) const;

		virtual eEShape GetShapeType( ) const;		

	protected:
		ArkPlane m_Planes[6];
			
	};
};
//--------------------------------------------------------------------------------
#endif // Frustum3f_h
